if (! process.env.NODE_ENV) process.env.NODE_ENV = 'production';

var AuthClient = require('crp-auth-client');
var TaskClient = require('crp-task-client');
var TaskProducerClient = require('crp-task-producer-client');

var fs = require('fs');
var plucker = require('png-plucker');
var spawn = require('child_process').spawn;

var LZString = require('./pre/lib/LZString');

// Authentication credentials
var userEmail = 'email';
var userPwd = 'password';

//Authentication using your CrowdProcess login information
AuthClient.login(userEmail, userPwd, function(err, credential) {
	if (err) throw err;

	//Options for creating task and dataunit stream
  //Change program.js
  	var options = {
	   bid: 1,
	   program: fs.readFileSync('./crowdprocess/build/aux-crowdprocess.js', 'utf8'), //Reads source code for Run(data) function from file
	   credential: credential
	};

	createTask(options);
});

function createTask(options) {

	var taskClient = TaskClient({
    	credential: options.credential
    });

	//Create CrowdProcess task
	taskClient.tasks.create({
    	bid: options.bid,
    	program: options.program
    }, afterTaskCreated);

	function afterTaskCreated(err, task) {
    if (err) throw err;

    //Create dataunit stream to send dataunits directly to CrowdProcess
    var stream = TaskProducerClient({
      credential: options.credential,
      taskId: task._id
    });

    console.log("TaskID: ", task._id);

    //Catch faults or errors emited by CrowdProcess
    stream.on('error', error);
    stream.on('fault', error);

    //Counter for sent and received dataunits
    var sent = 0;
    var received = 0;
    var counter = 0;

    var ffmpeg = spawn('ffmpeg', ['-i', './video/video.mp4', '-y', '-f', 'image2pipe', '-']);

    var obj = {};
    obj.input = {};
    obj.input.argv = [];
    obj.input.files = [];

    
    
    plucker(ffmpeg.stdout, function (error, image) {

      counter++;
      console.log(counter);
      // fs.writeFile('./img/file'+counter+'.png', image);
      console.dir(image);
      var name = 'video.' + counter + '.jpg';
      var content = LZString.compress(image.toString('binary'));

      obj.input.argv.push({arg:name, pos:obj.input.argv.length});
      obj.input.files.push({name:name, content:content});

     
      	if(sent == 0){
      		console.log("write")
        	stream.write(obj);
      	}

        sent++;

        obj.input.argv = [];
        obj.input.files = [];     

      

    });

    //Receive results from CrowdProcess
    stream.on('result', function(data) {

    	received++;

      for(name in data.output.files) {

	    	fs.writeFileSync("./results_" + name, data.output.files[name], 'binary');

      }  	

    });

    //Result stream from CrowdProcess ended
    stream.once('end', function() {

    	//Did not receive all dataunits
    	if (sent != received) console.error('Only received %d of the %d dataunits sent.', received, sent);

    });

	}

}

function error(err) {
  console.dir(err);
}