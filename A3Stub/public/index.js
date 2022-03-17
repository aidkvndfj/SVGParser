// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function () {
    // On page-load AJAX Example
    jQuery.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/uploadSVG',        //The server endpoint we are connecting to
        success: function (data) {
            if (data.length === 0) {
                $('#table').append("<tr><td>No Files</td></tr>");
            } else {
                for (let i = 0; i < data.length; i++) {
                    $('#fileLogTable').append("<tr><td><a download='" + data[i][0] + "' href='uploads/" + data[i][0] + "' title='" + data[i][0] + "'><img width=200px src='/uploads/" + data[i][0] + "'></a></td><td><a download='" + data[i][0] + "' href='uploads/" + data[i][0] + "' title='" + data[i][0] + "'>" + data[i][0] + "</a></td><td>" + data[i][1] + "KB</td><td>" + data[i][2] + "</td><td>" + data[i][3] + "</td><td>" + data[i][4] + "</td><td>" + data[i][5] + "</td></tr>");

                }
            }

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#table').append("<tr><td>Couldn't load files</td></tr>")
            console.log(error);
        }
    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function (e) {
        $('#blah').html("Form has data: " + $('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $('#uploadForm').submit(function (e) {
        console.log("Upload Success");
    });

    $('dropdown').change(function () {
        let fileName = $('dropdown option:selected').text();
        console.log(fileName);
    })

});