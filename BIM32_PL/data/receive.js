let config = {};

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}

function tq_unblock(n){
  if($(`#tq${n}`).is(':checked')){
    $(`#mqttT${n}`).removeAttr('disabled');
  }
  else{
    $(`#mqttT${n}`).attr('disabled', true);
  }
}

$(function(){
  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }
  });

  $('#mqttrcv').click(function(){
    if($('#mqttrcv').is(':checked')) $('#mqtt_recv').show(500);
    else $('#mqtt_recv').hide(500);
  });

  $('#thngrcv').click(function(){
    if($('#thngrcv').is(':checked')) $('#thng_recv').show(500);
    else $('#thng_recv').hide(500);
  });

  for(let i=1; i<9; i++){
    $(`#tq${i}`).click(function(){
      tq_unblock(i);
    });
  }

  $.getJSON(`config.json?R=${Math.random()}`,function(json){
    config = json;
    for(var b in config){
      try{
        $(`#${b}`).val(config[b]);
      }
      catch(b){}
    }
    $('#mqttrcv').attr('checked', config.mqttrcv);
    $('#thngrcv').attr('checked', config.thngrcv);
    for(let i=1; i<9; i++) $(`#tq${i}`).attr('checked', eval(`config.tq${i}`));
    if(config.mqttrcv) $('#mqtt_recv').show(500);
    if(config.thngrcv) $('#thng_recv').show(500);
    for(let i=1; i<9; i++){
      tq_unblock(i);
    }
    $('#loading').removeClass('active');
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    $('#save').text("Wysyłanie...");
    $('#save').css("background-color", "#FA0");
    config.mqttrcv = $('#mqttrcv').is(':checked');
    config.mqttServer = $('#mqttServer').val();
    config.mqttPort = Number($('#mqttPort').val());
    config.mqttLogin = $('#mqttLogin').val();
    config.mqttPass = $('#mqttPass').val();
    config.tq1 = $('#tq1').is(':checked');
    config.tq2 = $('#tq2').is(':checked');
    config.tq3 = $('#tq3').is(':checked');
    config.tq4 = $('#tq4').is(':checked');
    config.tq5 = $('#tq5').is(':checked');
    config.tq6 = $('#tq6').is(':checked');
    config.tq7 = $('#tq7').is(':checked');
    config.tq8 = $('#tq8').is(':checked');
    config.mqttT1 = $('#mqttT1').val();
    config.mqttT2 = $('#mqttT2').val();
    config.mqttT3 = $('#mqttT3').val();
    config.mqttT4 = $('#mqttT4').val();
    config.mqttT5 = $('#mqttT5').val();
    config.mqttT6 = $('#mqttT6').val();
    config.mqttT7 = $('#mqttT7').val();
    config.mqttT8 = $('#mqttT8').val();
    config.mto = Number($('#mto').val());
    config.mho = Number($('#mho').val());
    config.mpo = Number($('#mpo').val());
    config.mti = Number($('#mti').val());
    config.mhi = Number($('#mhi').val());
    config.mli = Number($('#mli').val());
    config.thngrcv = $('#thngrcv').is(':checked');
    config.chid = Number($('#chid').val());
    config.wrkey = $('#wrkey').val();
    config.rdkey = $('#rdkey').val();
    config.tto = Number($('#tto').val());
    config.tho = Number($('#tho').val());
    config.tpo = Number($('#tpo').val());
    config.tti = Number($('#tti').val());
    config.thi = Number($('#thi').val());
    config.tli = Number($('#tli').val());
    config.tbt = Number($('#tbt').val());
    $.ajax({
	    url: 'esp/save.php',
	    method: 'post',
      data: `CONFIG=${JSON.stringify(config, null, 2)}`,
	    success: function(answ){
        if(answ != "OK") alert(answ);
        $('#loading').removeClass('active');
        $('#save').css("background-color", "#AF0");
        $('#save').text("Zapisane");
        setTimeout(function(){
          $('#save').css("background-color", "#F1F1F1");
          $('#save').text("Zapisz");
        }, 3000);
	    }
    });
    return false;
  });
});
