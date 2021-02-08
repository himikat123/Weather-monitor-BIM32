let config = {}, error = false;

function logout(){
  $.ajax({
    type:"POST",
    url:"esp/logout.php",
    cache:false
  }).done();
  document.cookie='auth=0';
  window.location='http://radiokot.ru';
}

$(function(){
  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }
  });

  $('#loading').removeClass('active');

  $('#getfile').click(function(){
    var link = document.createElement('a');
    link.setAttribute('href', 'config.json');
    link.setAttribute('download', 'config.json');
    onload = link.click();
  });

  $('#configfile').on('change', function(e){
    $('#fnm').text(e.target.files[0].name);
    var reader = new FileReader();
    reader.onload = function(){
      var data = reader.result;
      try{config = JSON.parse(data);}
      catch(e){
        error = true;
        $('#restore').attr('disabled', true);
        alert("Invalid file selected");
      }
    };
    reader.readAsText($("#configfile")[0].files[0], "UTF-8");
    $('#restore').attr('disabled', false);
  });

  $('#restore').click(function(){
    $('#loading').addClass('active');
    $.ajax({
	    url: 'esp/save.php',
	    method: 'post',
      data: `CONFIG=${JSON.stringify(config, null, 2)}`,
	    success: function(answ){
        if(answ != "OK") alert(answ);
        $('#loading').removeClass('active');
        $('#fnm').text('');
        $('#restore').attr('disabled', true);
	    }
    });
    return false;
  });
});
