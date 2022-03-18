'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
    //Feel free to change the contents of style.css to prettify your Web app
    res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
    fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
        const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
        res.contentType('application/javascript');
        res.send(minimizedContents._obfuscatedCode);
    });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
    if (!req.files) {
        return res.status(400).send('No files were uploaded.');
    }

    let uploadFile = req.files.uploadFile;

    // Use the mv() method to place the file somewhere on your server
    uploadFile.mv('uploads/' + uploadFile.name, function (err) {
        if (err) {
            return res.status(500).send(err);
        }

        res.redirect('/');
    });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
    fs.stat('uploads/' + req.params.name, function (err, stat) {
        if (err == null) {
            res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
        } else {
            console.log('Error in file downloading route: ' + err);
            res.send('');
        }
    });
});

//******************** Your code goes here ******************** 
const svgParser = ffi.Library('./parser/bin/libsvgparser.so', {
    'rectsInSVG': ['int', ['String', 'String']], // Get the total rects in a filename svg
    'circsInSVG': ['int', ['String', 'String']], // get the total circles in a filename svg
    'pathsInSVG': ['int', ['String', 'String']], // get the total paths in a filename svg
    'groupsInSVG': ['int', ['String', 'String']], // Get the total groups in a filename svg
    'isValidSVG': ['int', ['String', 'String']], // check if a given filename svg is a valid svg
    'validSVGToJSON': ['String', ['String', 'String']], // translate a svg attributes to a JSON string
    'requestSVGTitle': ['String', ['String', 'String']], // get the title of a filename svg
    'requestSVGDesc': ['String', ['String', 'String']], // get the desc of a filename svg
    'requestSVGRects': ['String', ['String', 'String']], // get the rect list of a filename svg as a json
    'requestSVGCircles': ['String', ['String', 'String']], // get the circle list of a filename svg as a json
    'requestSVGPaths': ['String', ['String', 'String']], // get the path list of a filename svg as a json
    'requestSVGGroups': ['String', ['String', 'String']], // get the group list of a filename svg as a json
});

app.get('/SVGtoJSON/:fileName', function (req, res) {
    let fileName = req.params.fileName;

    if (svgParser.isValidSVG('uploads/' + fileName, "svg.xsd")) {
        let catFileName = 'uploads/' + fileName;
        let xsd = "svg.xsd";
        let totalJSON = '[' + svgParser.requestSVGRects(catFileName, xsd) + ',' + svgParser.requestSVGCircles(catFileName, xsd) + ']';
        res.send({ Title: svgParser.requestSVGTitle(catFileName, xsd), Desc: svgParser.requestSVGDesc(catFileName, xsd), Attrs: totalJSON });
    }
});

app.get('/uploadSVG', function (req, res) {
    console.log("Starting upload SVG");
    let allFiles = fs.readdirSync('./uploads');
    let nextIndex = 0;
    var allSVGs = [];

    for (let i = 0; i < allFiles.length; i++) {
        if (allFiles[i].includes('.svg') && svgParser.isValidSVG("uploads/" + allFiles[i], 'svg.xsd') == 1) {
            var fileInfo = fs.statSync("uploads/" + allFiles[i]);
            var size = fileInfo['size'];
            let parsedJSON = JSON.parse(svgParser.validSVGToJSON("uploads/" + allFiles[i], 'svg.xsd'));
            allSVGs[nextIndex] = [allFiles[i], Math.round(size / 1024), parsedJSON['numRect'], parsedJSON['numCirc'], parsedJSON['numPaths'], parsedJSON['numGroups']];
            nextIndex += 1;
        }
    }

    console.log("Ending upload SVG");
    res.send(allSVGs);
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);