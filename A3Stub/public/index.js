// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function () {


    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/uploadSVG',        //The server endpoint we are connecting to
        success: function (data) {
            if (data.length === 0) {
                $('#fileLogTable').append("<tr><td>No Files</td></tr>");
            } else {
                for (let i = 0; i < data.length; i++) {
                    if (data[i][0] == "ERROR:INVALID_SVG") {
                        if (confirm(`${data[i][1]} is a invalid SVG, would you like to delete it?`)) {
                            $.ajax({
                                type: 'post', url: '/removeUpload/' + data[i][1],
                                fail: function (error) {
                                    console.log(error);
                                }
                            });
                        }
                    } else {
                        $('#fileLogTable').append(`<tr><td><a download='${data[i][0]}' href='uploads/${data[i][0]}' title='${data[i][0]}'><img width=200px src='/uploads/${data[i][0]}'></a></td><td><a download='${data[i][0]}' href='uploads/${data[i][0]}' title='${data[i][0]}'>${data[i][0]}</a></td><td>${data[i][1]}KB</td><td>${data[i][2]}</td><td>${data[i][3]}</td><td>${data[i][4]}</td><td>${data[i][5]}</td></tr>`);

                        if (i == 0) {
                            $('#SVGDropDown').append("<option selected>" + data[i][0] + "</option>");
                            $('#shapeSVGDropDown').append("<option selected>" + data[i][0] + "</option>");
                        } else {
                            $('#SVGDropDown').append("<option>" + data[i][0] + "</option>");
                            $('#shapeSVGDropDown').append("<option>" + data[i][0] + "</option>");
                        }

                    }
                }
                SVGDropChange();
                tableSVGDropChange();
            }

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#fileLogTable').append("<tr><td>Couldn't load files</td></tr>")
            alert(`Couldn't load files`);
            console.log(error);
        }
    });

    $('#uploadForm').submit(function (e) {
        // $.ajax({
        //     type: 'post',
        //     dataType: 'string',
        //     url: '/upload',
        //     encType: "multipart/form-data",
        //     statusCode: {
        //         400: function () {
        //             alert(`No FIle To Upload\n`);
        //         }
        //     },
        //     success: function () {
        //         console.log("Upload Success");
        //     }
        // })
    });

    function SVGDropChange() {
        let fileName = $('#SVGDropDown option:selected').text();
        if (fileName == "None") {
            $('#viewPanelImage').attr('src', '');
            $('#viewPanelBody').html("");
            $('#SVGDesc').html("");
            $('#SVGTitle').html("");
        } else {
            $('#viewPanelImage').attr('src', fileName);

            $.ajax({
                type: 'get',            //Request type
                dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/SVGtoJSON/' + fileName,        //The server endpoint we are connecting to
                success: function (data) {
                    console.log(data);

                    if (data["Title"] == "ERROR:INVALID_SVG") {
                        if (confirm(`${data["Title"]} is a invalid SVG, would you like to delete it?`)) {
                            $.ajax({
                                type: 'post', url: '/removeUpload/' + data["Title"],
                                fail: function (error) {
                                    console.log(error);
                                }
                            });
                        }
                    } else {

                        $('#SVGTitle').html("");
                        $('#SVGTitle').append(data["Title"]);
                        $('#SVGDesc').html("");
                        $('#SVGDesc').append(data["Desc"]);

                        let parsedAttr = JSON.parse(data["Attrs"]);
                        let parsedRects = parsedAttr[0];
                        let parsedCircles = parsedAttr[1];
                        let parsedPaths = parsedAttr[2];
                        let parsedGroups = parsedAttr[3];
                        console.log(fileName);
                        console.log(data["Title"]);
                        console.log(data["Desc"]);
                        console.log(parsedRects);
                        console.log(parsedCircles);
                        console.log(parsedPaths);
                        console.log(parsedGroups);

                        let compIsEmpty = true
                        $('#viewPanelBody').html("");
                        $('#CompDropDown').html('');
                        for (let i = 0; i < parsedRects.length; i++) {
                            if (compIsEmpty) {
                                compIsEmpty = false;
                                $('#CompDropDown').append(`<option selected>Rectangle ${i + 1}</option>`)
                            } else
                                $('#CompDropDown').append(`<option>Rectangle ${i + 1}</option>`)
                            $('#viewPanelBody').append(`<tr><td>Rectangle ${i + 1}</td><td>Upper Left Corner: x = ${parsedRects[i]['x']}${parsedRects[i]['units']}, y = ${parsedRects[i]['y']}${parsedRects[i]['units']}\nWidth: ${parsedRects[i]['w']}${parsedRects[i]['units']}, Height: ${parsedRects[i]['h']}${parsedRects[i]['units']}</td><td>${parsedRects[i]['numAttr']}</td></tr>`)
                        }

                        for (let i = 0; i < parsedCircles.length; i++) {
                            if (compIsEmpty) {
                                compIsEmpty = false;
                                $('#CompDropDown').append(`<option selected>Circle ${i + 1}</option>`)
                            } else
                                $('#CompDropDown').append(`<option>Circle ${i + 1}</option>`)
                            $('#viewPanelBody').append(`<tr><td>Circle ${i + 1}</td><td>Centre: x = ${parsedCircles[i]['cx']}${parsedCircles[i]['units']}, y = ${parsedCircles[i]['cy']}${parsedCircles[i]['units']}, Radius = ${parsedCircles[i]['r']}${parsedCircles[i]['units']}</td><td>${parsedCircles[i]['numAttr']}</td></tr>`);
                        }

                        for (let i = 0; i < parsedPaths.length; i++) {
                            if (compIsEmpty) {
                                compIsEmpty = false;
                                $('#CompDropDown').append(`<option selected>Path ${i + 1}</option>`)
                            } else
                                $('#CompDropDown').append(`<option>Path ${i + 1}</option>`)
                            $('#viewPanelBody').append(`<tr><td>Path ${i + 1}</td><td>Data: ${parsedPaths[i]['d']}</td><td>${parsedPaths[i]['numAttr']}</td></tr>`);
                        }

                        for (let i = 0; i < parsedGroups.length; i++) {
                            if (compIsEmpty) {
                                compIsEmpty = false;
                                $('#CompDropDown').append(`<option selected>Group ${i + 1}</option>`)
                            } else
                                $('#CompDropDown').append(`<option>Group ${i + 1}</option>`)
                            $('#viewPanelBody').append(`<tr><td>Group ${i + 1}</td><td>Children: ${parsedGroups[i]['children']}</td><td>${parsedGroups[i]['numAttr']}</td></tr>`)
                        }
                        if (!compIsEmpty)
                            CompDropChange();
                    }

                },
                fail: function (error) {
                    // Non-200 return, do something with error
                    $('#viewPanelImage').append("<tr><td>Couldn't load files</td></tr>");
                    alert(`Couldn't load files`);
                    console.log(error);
                }
            });
        }
    }

    $('#SVGDropDown').change(function () {
        SVGDropChange();
    });

    function CompDropChange() {
        let svgFileName = $('#SVGDropDown option:selected').text();
        let wholeComponent = $('#CompDropDown option:selected').text();
        console.log(wholeComponent);
        console.log(svgFileName);

        console.log(wholeComponent.split(" ")[0]);

        $('#changeAttrDropBox').html('');
        switch (wholeComponent.split(" ")[0]) {
            case "Rectangle":
                $('#changeAttrDropBox').append(`<option>x</option`);
                $('#changeAttrDropBox').append(`<option>y</option`);
                $('#changeAttrDropBox').append(`<option>width</option`);
                $('#changeAttrDropBox').append(`<option>height</option`);
                break;

            case "Circle":
                $('#changeAttrDropBox').append(`<option>cx</option`);
                $('#changeAttrDropBox').append(`<option>cy</option`);
                $('#changeAttrDropBox').append(`<option>r</option`);
                break;

            case "Path":
                $('#changeAttrDropBox').append(`<option>data</option`);
                break;

            default:
                console.log(`${wholeComponent.split(" ")[0]} is a invalid type`);
                break;
        }

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: './CompToJSON/',
            data: {
                fileName: svgFileName,
                comp: wholeComponent
            },
            success: function (data) {
                $('#attrTableBody').html('');

                for (let i = 0; i < data.length; i++) {
                    $('#changeAttrDropBox').append(`<option>${data[i]['name']}</option>`);
                    $('#attrTableBody').append(`<tr><td>${data[i]['name']}</td><td>${data[i]['value']}</td></tr>`);
                }
            },
            fail: function (err) {
                // Non-200 return, do something with error
                $('#attrTableBody').append("<tr><td>Couldn't load Attributes</td></tr>");
                alert(`Couldn't load attributes`);
                console.log(error);
            }
        })
    }

    $('#CompDropDown').change(function () {
        CompDropChange();
    });

    document.getElementById('changeAttrButton').onclick = function () {
        let userInput = document.getElementById('charAttrInput').value
        let attribute = $('#changeAttrDropBox option:selected').text();
        console.log(`Change ${attribute} with ${userInput}`);

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: './putAttribute/',
            data: {
                input: userInput,
                element: $('#CompDropDown option:selected').text(),
                attr: attribute,
                fileName: $('#SVGDropDown option:selected').text(),
            },
            success: function (data) {
                switch (data['result']) {
                    case 'success':
                        alert("Good Change");
                        location.reload();
                        // CompDropChange();
                        break;

                    case 'ERROR:NOT_FOUND':
                        alert(`No attribute ${attribute}`);
                        break;

                    case 'ERROR:INVALID_SVG':
                        alert(`${userInput} is invalid for ${attribute}`);
                        break;

                    default:
                        alert("you should never see this <3");
                        break;
                }
            },
            fail: function (err) {
                console.log(err);
            }
        });
    };

    document.getElementById('changeTitleButton').onclick = function () {
        let userInput = document.getElementById('changeTitleInput').value
        let element = $('#changeTitleDropBox option:selected').text();
        console.log(`Change ${element} with ${userInput}`);

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: './putTitle/',
            data: {
                input: userInput,
                element: element,
                fileName: $('#SVGDropDown option:selected').text(),
            },
            success: function (data) {
                switch (data['result']) {
                    case 'success':
                        alert("Good Change");
                        location.reload();
                        break;

                    case 'ERROR:INVALID_TITLE':
                    case 'ERROR:INVALID_DESCRIPTION':
                        alert(`${userInput} make ${fileName} a invalid SVG.`);
                        break;

                    default:
                        alert("you should never see this <3");
                        break;
                }
            },
            fail: function (err) {
                console.log(err);
            }
        });
    };

    $('#createSVGForms').submit(function (data) {
        data.preventDefault();
        let svgName = $('#svgName').val();
        let svgTitle = $('#svgTitle').val();
        let svgDesc = $('#svgDesc').val();
        let xPos = $('#xPos').val();
        let yPos = $('#yPos').val();
        let rectHeight = $('#rectHeight').val();
        let rectWidth = $('#rectWidth').val();
        let cxPos = $('#cxPos').val();
        let cyPos = $('#cyPos').val();
        let radius = $('#radius').val();
        let pathData = $('#pathData').val();

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/createSVG/',
            data: {
                svgName: svgName,
                svgTitle: svgTitle,
                svgDesc: svgDesc,
                xPos: xPos,
                yPos: yPos,
                rectHeight: rectHeight,
                rectWidth: rectWidth,
                cxPos: cxPos,
                cyPos: cyPos,
                radius: radius,
                pathData: pathData,
            },
            success: function (data) {
                console.log(data['response']);
                switch (data['response']) {
                    case 'success':
                        alert(`${svgName} successfully created`);
                        location.reload();
                        break;

                    case 'ERROR:EXISTING_SVG':
                        alert(`${svgName} already exists`);
                        break;

                    case 'ERROR:INVALID_DATA':
                        alert(`${pathData} is invalid path data`);
                        break;

                    case 'ERROR:INVALID_XVAL':
                        alert(`${xPos} is invalid rect x`);
                        break;

                    case 'ERROR:INVALID_YVAL':
                        alert(`${yPos} is invalid rect y`);
                        break;

                    case 'ERROR:INVALID_WIDTH':
                        alert(`${rectWidth} is invalid rect width`);
                        break;

                    case 'ERROR:INVALID_HEIGHT':
                        alert(`${rectHeight} is invalid rect height`);
                        break;

                    case 'ERROR:INVALID_CENTERX':
                        alert(`${cxPos} is invalid circle center x pos`);
                        break;

                    case 'ERROR:INVALID_CENTERY':
                        alert(`${cyPos} is invalid circle center y pos`);
                        break;

                    case 'ERROR:INVALID_RADIUS':
                        alert(`${radius} is invalid circle radius`);
                        break;


                    default:
                        alert(`new error ${data['response']} or something went very wrong`);
                        break;
                }
            },
            fail: function (err) {
                console.log(err);
                alert(err);
            }
        });
    });

    function tableSVGDropChange() {
        let shapeType = $('#shapeTypeDropDown option:selected').text();
        $('#dynamicMenu').html('');

        switch (shapeType) {
            case 'Rectangle':
                $('#dynamicMenu').append(`<tr>
                <td colspan="2">Add Rect</td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addXPos">X Position:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addXPos" name="addXPos" value="0"
                        size="40"></td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addYPos">Y Position:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addYPos" name="addYPos" value="0"
                        size="40"></td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addRectHeight">Height:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addRectHeight" name="addRectHeight"
                        value="0" size="40"></td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addRectWidth">Width:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addRectWidth" name="addRectWidth" value="0"
                        size="40"></td>
            </tr>`)
                break;

            case 'Circle':
                $('#dynamicMenu').append(`<tr>
                <td colspan="2">Add Circle</td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addCXPos">X Position:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addCXPos" name="addCXPos" value="0"
                        size="40"></td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addCYPos">Y Position:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addCYPos" name="addCYPos" value="0"
                        size="40"></td>
            </tr>
            <tr>
                <td colspan="1" style="width: 30%;"><label for="addRadius">Radius:</label></td>
                <td colspan="1" style="width: 70%;"><input type="text" id="addRadius" name="addRadius" value="0"
                        size="40"></td>
            </tr>`);
                break;


            default:
                alert("YOU SHOULD NEVER GET HERE <3");
                break;
        }
    }

    $('#shapeTypeDropDown').change(function () {
        tableSVGDropChange();
    });

    $('#addShapeForm').submit(function (data) {
        data.preventDefault();
        let fileName = $('#shapeSVGDropDown option:selected').text();
        let type = $('#shapeTypeDropDown option:selected').text();

        // console.log($('#addXPos').val());

        switch (type) {
            case 'Rectangle':
                var dataJSON = {
                    xPos: $('#addXPos').val(),
                    yPos: $('#addYPos').val(),
                    rectHeight: $('#addRectHeight').val(),
                    rectWidth: $('#addRectWidth').val(),
                }
                break;

            case 'Circle':
                var dataJSON = {
                    cxPos: $('#addCXPos').val(),
                    cyPos: $('#addCYPos').val(),
                    radius: $('#radius').val(),
                }
                break;
        }

        console.log(dataJSON);
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/addShape/',
            data: {
                fileName: fileName,
                type: type,
                dataJSON: dataJSON,
            },
            success: function (data) {
                switch (data['result']) {
                    case 'success':
                        alert(`${type} successfully added to ${fileName}`);
                        location.reload();
                        break;

                    case 'ERROR:INVALID_SVG_CREATED':
                        alert(`${type} was not successfully added to ${fileName}, would create invalid SVG`);
                        break;

                    case 'ERROR:BROKEN':
                        alert(`Something very bad has occured`);

                    default:
                        alert('Something very very worse has occurred');
                }
            }
        });
    });

});