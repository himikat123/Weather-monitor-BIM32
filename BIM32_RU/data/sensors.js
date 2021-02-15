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
  });

  setInterval(function(){
    $.ajax({
	    url: 'esp/sensors.php',
	    method: 'get',
	    dataType: 'json',
	    success: function(data){
        for(var b in data){
          let cr = b.slice(1);
          if(data[b] == 40400) $(`#${b}`).html('--');
          else $(`#${b}`).html((Number(data[b]) + Number($(`#${cr}`).val())).toFixed(1));
        }
        $('#loading').removeClass('active');
	    }
    });
  },5000);

  $('form').submit(function(){
    $('#loading').addClass('active');
    $('#save').text("Отправка...");
    $('#save').css("background-color", "#FA0");
    config.bmet = Number($('#bmet').val());
    config.bmpt = Number($('#bmpt').val());
    config.shtt = Number($('#shtt').val());
    config.dhtt = Number($('#dhtt').val());
    config.bmeh = Number($('#bmeh').val());
    config.shth = Number($('#shth').val());
    config.dhth = Number($('#dhth').val());
    config.bmep = Number($('#bmep').val());
    config.bmpp = Number($('#bmpp').val());
    config.max  = Number($('#max').val());
    config.bh   = Number($('#bh').val());
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
