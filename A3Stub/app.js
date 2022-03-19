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

    let uploadedFiles = fs.readdirSync('./uploads');
    let isAdded = false;

    for (let i = 0; i < uploadedFiles.length; i++) {
        if (uploadedFiles[i] == uploadFile.name) {
            isAdded = true;
        }
    }

    if (!isAdded) {
        uploadFile.mv('uploads/' + uploadFile.name, function (err) {
            if (err) {
                return res.status(500).send(err);
            }

            res.redirect('/');
        });
    } else {
        return res.status(500);
    }
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
app.post('/removeUpload/:fileName', function (req, res) {
    fs.unlinkSync(`uploads/${req.params.fileName}`);
});

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
    'requestIndexRectAttrs': ['String', ['String', 'int']], // Get the attributes from a single rect struct
    'requestIndexCircAttrs': ['String', ['String', 'int']], // Get the attributes from a single circle struct
    'requestIndexPathAttrs': ['String', ['String', 'int']], // Get the attributes from a single path struct
    'requestIndexGroupAttrs': ['String', ['String', 'int']], // Get the attributes from a single group struct
    'setCircAttribute': ['String', ['String', 'int', 'String', 'String']],
});

app.get('/SVGtoJSON/:fileName', function (req, res) {
    console.log("Starting SVGtoJSON");
    let fileName = req.params.fileName;

    if (svgParser.isValidSVG('uploads/' + fileName, "svg.xsd")) {
        let catFileName = 'uploads/' + fileName;
        let xsd = "svg.xsd";
        let totalJSON = '[' + svgParser.requestSVGRects(catFileName, xsd) + ',' + svgParser.requestSVGCircles(catFileName, xsd) + ',' + svgParser.requestSVGPaths(catFileName, xsd) + ',' + svgParser.requestSVGGroups(catFileName, xsd) + ']';
        res.send({ Title: svgParser.requestSVGTitle(catFileName, xsd), Desc: svgParser.requestSVGDesc(catFileName, xsd), Attrs: totalJSON });
    } else {
        res.send({ Title: "ERROR:INVALID_SVG", Desc: fileName });
    }

    console.log("Ending SVGtoJSON");
});

app.get('/CompToJSON/', function (req, res) {
    let fileName = req.query.fileName;
    let wholeComponent = req.query.comp;
    let component = wholeComponent.split(" ");

    console.log("Starting CompToJSON");

    // console.log(fileName);
    // console.log(component);

    switch (component[0]) {
        case "Rectangle":
            res.send(svgParser.requestIndexRectAttrs('uploads/' + fileName, component[1]));
            break;

        case "Circle":
            res.send(svgParser.requestIndexCircAttrs('uploads/' + fileName, component[1]));
            break;

        case "Path":
            res.send(svgParser.requestIndexPathAttrs('uploads/' + fileName, component[1]));
            break;

        case "Group":
            res.send(svgParser.requestIndexGroupAttrs('uploads/' + fileName, component[1]));
            break;

        default:
            console.log(`${component[0]} is a invalid type`);
            break;
    }
    console.log("Ending CompToJSON");
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
            allSVGs[i] = [allFiles[i], Math.round(size / 1024), parsedJSON['numRect'], parsedJSON['numCirc'], parsedJSON['numPaths'], parsedJSON['numGroups']];
        } else {
            allSVGs[i] = ["ERROR:INVALID_SVG", allFiles[i]];
            console.log(`${allFiles[i]} is a invalid SVG`);
        }
    }

    console.log("Ending upload SVG");
    res.send(allSVGs);
});

app.get('/putAttribute/', function (req, res) {
    let input = req.query.input;
    let attribute = req.query.attr;
    let fileName = req.query.fileName;
    let component = req.query.element;
    let splitComp = component.split(" ");
    console.log(component, fileName, input, attribute);

    switch (splitComp[0]) {
        case "Rectangle":

            break;

        case "Circle":
            var result = svgParser.setCircAttribute('uploads/' + fileName, splitComp[1], attribute, input);
            break;

        case "Path":

            break;

        case "Group":

            break;

        default:
            console.log(`${splitComp[0]} is a invalid type`);
            break;
    }

    res.send({ result: result });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);