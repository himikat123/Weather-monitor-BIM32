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
    let now_date = new Date();
    $.ajax({
	    url: 'esp/wsensor.php',
	    method: 'get',
      data: `H=${now_date.getHours()}&I=${now_date.getMinutes()}&S=${now_date.getSeconds()}&D=${now_date.getDate()}&M=${Number(now_date.getMonth()) + 1}&Y=${now_date.getFullYear()}`,
	    dataType: 'json',
	    success: function(dt){
        data = dt;
        if(data.wtemp > -55 && data.wtemp < 100) $('#wtemp').text((Math.round((data.wtemp + Number($('#wtempc').val())) * 10) / 10).toFixed(1));
        else $('#wtemp').text('--');
        if(data.whum >= 0 && data.whum <= 100) $('#whum').text((Math.round((data.whum + Number($('#whumc').val())) * 10) / 10).toFixed(1));
        if(data.wpres > 500 && data.wpres < 1200) $('#wpres').text((Math.round((data.wpres + Number($('#wpresc').val())) * 10) / 10).toFixed(1));
        if(data.wlight >= 0 && data.wlight < 50000) $('#wlight').text((Math.round((data.wlight + Number($('#wlightc').val())) * 10) / 10).toFixed(1));
        if(data.adc > 0) $('#wbat').text((Math.round(data.adc / Number($('#wbatc').val()) * 100) / 100).toFixed(2));
        $('#wtemptp').text(data.wtemptp);
        $('#whumtp').text(data.whumtp);
        $('#wprestp').text(data.wprestp);
        $('#wlighttp').text(data.wlighttp);
        let dte = Date.parse(data.time);
        let w_dt = new Date(dte);
        let ndd = w_dt.getDate();
        let ndm = w_dt.getMonth();
        let ndy = w_dt.getFullYear();
        let mo = ['jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec'];
        if(ndy > 2020) $('#time').text(`${data.time.split(' ')[0]} ${ndd}.${mo[ndm]}.${ndy}`);
        else $('#time').text('--');
        $('#loading').removeClass('active');
	    }
    });
  },5000);

  $('form').submit(function(){
    $('#loading').addClass('active');
    $('#save').text("Sending...");
    $('#save').css("background-color", "#FA0");
    config.wtempc = Number($('#wtempc').val());
    config.whumc = Number($('#whumc').val());
    config.wpresc = Number($('#wpresc').val());
    config.wlightc = Number($('#wlightc').val());
    config.chnum = Number($('#chnum').val());
    config.wexpire = Number($('#wexpire').val());
    config.wbatc = Number($('#wbatc').val());
    config.battype = ($('input[name=battype]:checked').val() == "battery") ? 0 : 1;
    $.ajax({
	    url: 'esp/save.php',
	    method: 'post',
      data: `CONFIG=${JSON.stringify(config, null, 2)}`,
	    success: function(answ){
        if(answ != "OK") alert(answ);
        $('#loading').removeClass('active');
        $('#save').css("background-color", "#AF0");
        $('#save').text("Saved");
        setTimeout(function(){
          $('#save').css("background-color", "#F1F1F1");
          $('#save').text("Save");
        }, 3000);
	    }
    });
    return false;
  });

  $('#wtempc').change(function(){
    if(data.wtemp == undefined || data.wtemp < -55 || data.wtemp > 100) $('#wtemp').text('--');
    else $('#wtemp').text((Math.round((data.wtemp + Number($('#wtempc').val())) * 10) / 10).toFixed(1));
  });

  $('#whumc').change(function(){
    if(data.whum == undefined || data.whum < 0 || data.whum > 100) $('#whum').text('--');
    else $('#whum').text((Math.round((data.whum + Number($('#whumc').val())) * 10) / 10).toFixed(1));
  });

  $('#wpresc').change(function(){
    if(data.wpres == undefined || data.wpres < 500 || data.wpres > 1200) $('#wpres').text('--');
    else $('#wpres').text((Math.round((data.wpres + Number($('#wpresc').val())) * 10) / 10).toFixed(1));
  });

  $('#wlightc').change(function(){
    if(data.wlight == undefined || data.wlight < 0 || data.wtemp > 50000) $('#wlight').text('--');
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
