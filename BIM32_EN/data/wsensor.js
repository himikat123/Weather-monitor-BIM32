let config = {}, data = {};

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
    $('#wbat_c').val(300 - config.wbatc);
    $(`input:radio[value=${config.battype}]`)[0].checked = true;
  });

  setInterval(function(){
    $.ajax({
	    url: 'esp/wsensor.php',
	    method: 'get',
	    dataType: 'json',
	    success: function(dt){
        data = dt;
        $('#wtemp').text((Math.round((data.wtemp + Number($('#wtempc').val())) * 10) / 10).toFixed(1));
        $('#whum').text((Math.round((data.whum + Number($('#whumc').val())) * 10) / 10).toFixed(1));
        $('#wpres').text((Math.round((data.wpres + Number($('#wpresc').val())) * 10) / 10).toFixed(1));
        $('#wlight').text((Math.round((data.wlight + Number($('#wlightc').val())) * 10) / 10).toFixed(1));
        $('#wbat').text((Math.round(data.adc / Number($('#wbatc').val()) * 100) / 100).toFixed(2));
        $('#wtemptp').text(data.wtemptp);
        $('#whumtp').text(data.whumtp);
        $('#wprestp').text(data.wprestp);
        $('#wlighttp').text(data.wlighttp);
        $('#time').text(data.time);
        $('#loading').removeClass('active');
	    }
    });
  },5000);

  $('form').submit(function(){
    $('#loading').addClass('active');
    config.wtempc = $('#wtempc').val();
    config.whumc = $('#whumc').val();
    config.wpresc = $('#wpresc').val();
    config.wlightc = $('#wlightc').val();
    config.chnum = $('#chnum').val();
    config.wexpire = $('#wexpire').val();
    config.wbatc = $('#wbatc').val();
    config.battype = ($('input[name=battype]:checked').val() == "battery") ? 0 : 1;
    $.ajax({
	    url: 'esp/save.php',
	    method: 'post',
      data: `CONFIG=${JSON.stringify(config, null, 2)}`,
	    success: function(answ){
        if(answ != "OK") alert(answ);
        $('#loading').removeClass('active');
	    }
    });
    return false;
  });

  $('#wtempc').change(function(){
    if(data.wtemp == undefined) $('#wtemp').text('--');
    else $('#wtemp').text((Math.round((data.wtemp + Number($('#wtempc').val())) * 10) / 10).toFixed(1));
  });

  $('#whumc').change(function(){
    if(data.whum == undefined) $('#whum').text('--');
    else $('#whum').text((Math.round((data.whum + Number($('#whumc').val())) * 10) / 10).toFixed(1));
  });

  $('#wpresc').change(function(){
    if(data.wpres == undefined) $('#wpres').text('--');
    else $('#wpres').text((Math.round((data.wpres + Number($('#wpresc').val())) * 10) / 10).toFixed(1));
  });

  $('#wlightc').change(function(){
    if(data.wlight == undefined) $('#wlight').text('--');
    else $('#wlight').text((Math.round((data.wlight + Number($('#wlightc').val())) * 10) / 10).toFixed(1));
  });

  $('#wbatc').change(function(){
    let k = $('#wbatc').val();
    $('#wbat_c').val(300 - k);
    if(data.adc == undefined) $('#wbat').text('--');
    else $('#wbat').text((Math.round(data.adc / Number($('#wbatc').val()) * 100) / 100).toFixed(2));
  });

  $('#wbat_c').change(function(){
    let k = $('#wbat_c').val();
    $('#wbatc').val(300 - k);
    if(data.adc == undefined) $('#wbat').text('--');
    else $('#wbat').text((Math.round(data.adc / Number($('#wbatc').val()) * 100) / 100).toFixed(2));
  });
});
