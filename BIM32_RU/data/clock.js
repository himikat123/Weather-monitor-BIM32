let config = {};

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}

$(function(){
  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }
  });

  $.getJSON(`config.json?R=${Math.random()}`,function(json){
    config = json;
    for(var b in config){
      try{
        $(`#${b}`).val(config[b]);
      }
      catch(b){}
    }
    $(`input:radio[value=${config.dlst}]`)[0].checked = true;
    $('#loading').removeClass('active');
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    $('#save').text("Отправка...");
    $('#save').css("background-color", "#FA0");
    config.ntp = $('#ntp').val();
    config.utc = Number($('#utc').val());
    config.dlst = ($('input[name="dlst"]:checked').val() == "true") ? true : false;
    $.ajax({
	    url: 'esp/save.php',
	    method: 'post',
      data: `CONFIG=${JSON.stringify(config, null, 2)}`,
	    success: function(answ){
        if(answ != "OK") alert(answ);
        $('#loading').removeClass('active');
        $('#save').css("background-color", "#AF0");
        $('#save').text("Сохранено");
        setTimeout(function(){
          $('#save').css("background-color", "#F1F1F1");
          $('#save').text("Сохранить");
        }, 3000);
	    }
    });
    return false;
  });
});
