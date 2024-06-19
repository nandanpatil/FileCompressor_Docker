const { urlencoded } = require('body-parser');
const express = require('express')
const multer  = require('multer')
const { exec } = require('child_process');
const util = require('util');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT_NUMBER = 3000;
app.use( express.static( "public" ) );
const storage = multer.diskStorage({
    destination: function (req, file, cb) {
      cb(null, './uploads')
    },
    filename: function (req, file, cb) {
        var newfilename = file.originalname.replace(".txt",Date.now()+".txt");
      cb(null, newfilename)
    }
  })
  
const upload = multer({ storage: storage })

app.set("view engine","ejs");

app.use(express.json());
app.use(express.urlencoded({extended:false}));


app.get("/",(req,res)=>{
    res.render("home.ejs")
})


app.post("/download",(req,res)=>{
  console.log(req.body);
  res.send("Success");
})

app.get("/error",(req,res)=>{
  res.render("error");
})


const execPromise = util.promisify(exec);
app.post('/compress', upload.single('filee'), async (req, res) => {
  //console.log(req.file)
  try {
    if(req.file==undefined || req.file.originalname.substring(req.file.originalname.length-4)!=".txt"){
    //  console.log(req.file);
      if(req.file!=undefined){
      const uploadedFilePathd = "./uploads/"+req.file.originalname;
     // console.log(uploadedFilePathd);
      try {
        await fs.promises.unlink(uploadedFilePathd);
      } catch (unlinkErr) {
        console.error('Error deleting uploaded file:', unlinkErr);
      }
    }
      return res.redirect("/error");
     }
    const uploadedFilePath = "./uploads/"+req.file.filename;
    const command = `./pro -c ${uploadedFilePath}`;

    const { stdout, stderr } = await execPromise(command);

    if (stderr) {
      console.error('Execution error:', stderr);
      return res.status(500).send(`Error running C++ script: ${stderr}`);
    }

    const downloadFilePath = path.join(__dirname, 'downloads', req.file.filename.replace('.txt', '.cmp'));

    if (fs.existsSync(downloadFilePath)) {
      res.download(downloadFilePath, async (err) => {
        if (err) {
          console.error('Download error:', err);
          return res.status(500).send('Internal server error.');
        }
        else{
          try {
            await fs.promises.unlink(uploadedFilePath);
          } catch (unlinkErr) {
            console.error('Error deleting uploaded file:', unlinkErr);
          }
  
          try {
            await fs.promises.unlink(downloadFilePath);
          } catch (unlinkErr) {
            console.error('Error deleting output file:', unlinkErr);
          }
        }  
      });
    } else {
      console.error('File not found:', downloadFilePath);
      return res.status(404).send('File not found.');
    }
  } catch (error) {
    console.error('Error occurred:', error);
    return res.status(500).send({ message: error.message || 'Error occurred' });
  }
});

app.post('/decompress', upload.single('filee'), async (req, res) => {
  try {
    if(req.file==undefined || req.file.originalname.substring(req.file.originalname.length-4)!=".cmp"){
      if(req.file!=undefined){
        const uploadedFilePathd = "./uploads/"+req.file.originalname;
        try {
          await fs.promises.unlink(uploadedFilePathd);
        } catch (unlinkErr) {
          console.error('Error deleting uploaded file:', unlinkErr);
        }
      }
      return res.redirect("/error");
     }
   // console.log(req.file);
    const uploadedFilePath = "./uploads/"+req.file.filename;
   // console.log(uploadedFilePath);
    const command = `./pro -dc ${uploadedFilePath}`;
   // console.log(command)
    const { stdout, stderr } = await execPromise(command);

    if (stderr) {
      console.error('Execution error:', stderr);
      return res.status(500).send(`Error running C++ script: ${stderr}`);
    }

    const downloadFilePath = path.join(__dirname, 'downloads', req.file.filename.replace('.cmp', '.txt'));

    if (fs.existsSync(downloadFilePath)) {
      res.download(downloadFilePath, async (err) => {
        if (err) {
          console.error('Download error:', err);
          return res.status(500).send('Internal server error.');
        }
        else{
          try {
            await fs.promises.unlink(uploadedFilePath);
          } catch (unlinkErr) {
            console.error('Error deleting uploaded file:', unlinkErr);
          }
  
          try {
            await fs.promises.unlink(downloadFilePath);
          } catch (unlinkErr) {
            console.error('Error deleting output file:', unlinkErr);
          }
        }  
      });
    } else {
      console.error('File not found:', downloadFilePath);
      return res.status(404).send('File not found.');
    }
  } catch (error) {
    console.error('Error occurred:', error);
    return res.status(500).send({ message: error.message || 'Error occurred' });
  }
});


app.listen(3000,function(){
    console.log("Server running on PORT "+PORT_NUMBER)
})

