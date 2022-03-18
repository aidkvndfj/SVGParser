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
                    $('#fileLogTable').append("<tr><td><a download='" + data[i][0] + "' href='uploads/" + data[i][0] + "' title='" + data[i][0] + "'><img width=200px src='/uploads/" + data[i][0] + "'></a></td><td><a download='" + data[i][0] + "' href='uploads/" + data[i][0] + "' title='" + data[i][0] + "'>" + data[i][0] + "</a></td><td>" + data[i][1] + "KB</td><td>" + data[i][2] + "</td><td>" + data[i][3] + "</td><td>" + data[i][4] + "</td><td>" + data[i][5] + "</td></tr>");

                    $('#dropdown').append("<option>" + data[i][0] + "</option>");
                }
            }

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#fileLogTable').append("<tr><td>Couldn't load files</td></tr>")
            console.log(error);
        }
    });

    $('#uploadForm').submit(function (e) {
        console.log("Upload Success");
    });

    $('#dropdown').change(function () {
        let fileName = $('#dropdown option:selected').text();
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
                    let parsedAttr = JSON.parse(data["Attrs"]);
                    let parsedRects = parsedAttr[0];
                    let parsedCircles = parsedAttr[1];
                    console.log(parsedRects);
                    console.log(parsedCircles);

                    $('#viewPanelBody').html("");
                    for (let i = 0; i < parsedRects.length; i++) {
                        $('#viewPanelBody').append("<tr><td>Rectangle " + (i + 1) + "</td><td>Upper Left Corner: x = " + parsedRects[i]['x'] + parsedRects[i]['units'] + ", y = " + parsedRects[i]['y'] + parsedRects[i]['units'] + "\nWidth: " + parsedRects[i]['w'] + parsedRects[i]['units'] + ", Height: " + parsedRects[i]['h'] + parsedRects[i]['units'] + "</td><td>" + parsedRects[i]['numAttr'] + "</td></tr>")
                    }

                    for (let i = 0; i < parsedCircles.length; i++) {
                        $('#viewPanelBody').append("<tr><td>Circle " + (i + 1) + "</td><td>Centre: x = " + parsedCircles[i]['cx'] + parsedCircles[i]['units'] + ", y = " + parsedCircles[i]['cy'] + parsedCircles[i]['units'] + ", Radius = " + parsedCircles[i]['r'] + parsedCircles[i]['units'] + "</td><td>" + parsedCircles[i]['numAttr'] + "</td></tr>");
                    }

                    $('#SVGTitle').html("");
                    $('#SVGTitle').append(data["Title"]);
                    $('#SVGDesc').html("");
                    $('#SVGDesc').append(data["Desc"]);

                },
                fail: function (error) {
                    // Non-200 return, do something with error
                    $('#viewPanelImage').append("<tr><td>Couldn't load files</td></tr>");
                    console.log(error);
                }
            });
        }
    })

});