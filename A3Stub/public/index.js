// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
    // On page-load AJAX Example
    jQuery.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/uploads',        //The server endpoint we are connecting to
        success: function (data) {
            if (data.length === 0) {
                $('#table').append("<tr><td>No Files<tr><td");
            } else {
                for (var i = 0; i < data.length; i++) {
                    $('#table').append(<img src=data[i] width=200px>);
                }
            }

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $('#uploadForm').submit(function(e){
        console.log("Upload Success");
    });

    $('dropdown').change(function(){
        let fileName = $('dropdown option:selected').text();
        console.log(fileName);
    })

});