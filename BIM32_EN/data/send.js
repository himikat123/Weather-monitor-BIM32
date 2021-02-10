let config = {};

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}

function fq_unblock(n){
  if($(`#fq${n}`).is(':checked')){
    $(`#mqttF${n}`).removeAttr('disabled');
    $(`#fd${n}_s`).removeAttr('disabled');
    $(`#fd${n}_t`).removeAttr('disabled');
  }
  else{
    $(`#mqttF${n}`).attr('disabled', true);
    $(`#fd${n}_s`).attr('disabled', true);
    $(`#fd${n}_t`).attr('disabled', true);
  }
}

function fd_unblock(n){
  if($(`#fd${n}_s`).val() == 0){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 1){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', false);
  }
  if($(`#fd${n}_s`).val() == 2){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 3){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 4){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 5){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 6){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#fd${n}_s`).val() == 7){
    $(`#fd${n}_t option[value="0"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="1"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="2"]`).attr('disabled', true);
    $(`#fd${n}_t option[value="3"]`).attr('disabled', false);
    $(`#fd${n}_t option[value="4"]`).attr('disabled', true);
  }
}

function f_unblock(n){
  if($(`#f${n}_s`).val() == 0){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', false);
    $(`#f${n}_t option[value="2"]`).attr('disabled', false);
    $(`#f${n}_t option[value="3"]`).attr('disabled', true);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 1){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', false);
    $(`#f${n}_t option[value="2"]`).attr('disabled', false);
    $(`#f${n}_t option[value="3"]`).attr('disabled', false);
    $(`#f${n}_t option[value="4"]`).attr('disabled', false);
  }
  if($(`#f${n}_s`).val() == 2){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', false);
    $(`#f${n}_t option[value="2"]`).attr('disabled', false);
    $(`#f${n}_t option[value="3"]`).attr('disabled', true);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 3){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', true);
    $(`#f${n}_t option[value="2"]`).attr('disabled', false);
    $(`#f${n}_t option[value="3"]`).attr('disabled', true);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 4){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', false);
    $(`#f${n}_t option[value="2"]`).attr('disabled', true);
    $(`#f${n}_t option[value="3"]`).attr('disabled', true);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 5){
    $(`#f${n}_t option[value="0"]`).attr('disabled', false);
    $(`#f${n}_t option[value="1"]`).attr('disabled', false);
    $(`#f${n}_t option[value="2"]`).attr('disabled', true);
    $(`#f${n}_t option[value="3"]`).attr('disabled', true);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 6){
    $(`#f${n}_t option[value="0"]`).attr('disabled', true);
    $(`#f${n}_t option[value="1"]`).attr('disabled', true);
    $(`#f${n}_t option[value="2"]`).attr('disabled', true);
    $(`#f${n}_t option[value="3"]`).attr('disabled', false);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
  if($(`#f${n}_s`).val() == 7){
    $(`#f${n}_t option[value="0"]`).attr('disabled', true);
    $(`#f${n}_t option[value="1"]`).attr('disabled', true);
    $(`#f${n}_t option[value="2"]`).attr('disabled', true);
    $(`#f${n}_t option[value="3"]`).attr('disabled', false);
    $(`#f${n}_t option[value="4"]`).attr('disabled', true);
  }
}

$(function(){
  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }
  });

  $('#mqttsend').click(function(){
    if($('#mqttsend').is(':checked')) $('#mqtt_send').show(500);
    else $('#mqtt_send').hide(500);
  });

  $('#thngsend').click(function(){
    if($('#thngsend').is(':checked')) $('#thng_send').show(500);
    else $('#thng_send').hide(500);
  });

  $('#nrdmsend').click(function(){
    if($('#nrdmsend').is(':checked')) $('#nrdm_send').show(500);
    else $('#nrdm_send').hide(500);
  });

  for(let i=1; i<9; i++){
    $(`#fq${i}`).click(function(){
      fq_unblock(i);
    });
  }

  for(let i=1; i<9; i++){
    $(`#fd${i}_s`).click(function(){
      fd_unblock(i);
    });
  }

  for(let i=1; i<9; i++){
    $(`#f${i}_s`).click(function(){
      f_unblock(i);
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
    $('#mqttsend').attr('checked', config.mqttsend);
    $('#thngsend').attr('checked', config.thngsend);
    $('#nrdmsend').attr('checked', config.nrdmsend);
    for(let i=1; i<9; i++) $(`#fq${i}`).attr('checked', eval(`config.fq${i}`));
    for(let i=1; i<9; i++) $(`#fd${i}_s`).val(eval(`config.fd${i}[0]`));
    for(let i=1; i<9; i++) $(`#fd${i}_t`).val(eval(`config.fd${i}[1]`));
    for(let i=1; i<9; i++) $(`#f${i}_s`).val(eval(`config.f${i}[0]`));
    for(let i=1; i<9; i++) $(`#f${i}_t`).val(eval(`config.f${i}[1]`));
    if(config.mqttsend) $('#mqtt_send').show(500);
    if(config.thngsend) $('#thng_send').show(500);
    if(config.nrdmsend) $('#nrdm_send').show(500);
    for(let i=1; i<9; i++){
      fq_unblock(i);
      fd_unblock(i);
      f_unblock(i);
    }
  });

  $.getJSON(`esp/mac.php?R=${Math.random()}`, function(json){
    $('#mac_addr').val('BIM' + json.mac.replace(/:/g, ""));
    $('#loading').removeClass('active');
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    config.tupd = Number($('#tupd').val());
    config.mqttsend = $('#mqttsend').is(':checked');
    config.mqttServer = $('#mqttServer').val();
    config.mqttPort = Number($('#mqttPort').val());
    config.mqttLogin = $('#mqttLogin').val();
    config.mqttPass = $('#mqttPass').val();
    config.fq1 = $('#fq1').is(':checked');
    config.fq2 = $('#fq2').is(':checked');
    config.fq3 = $('#fq3').is(':checked');
    config.fq4 = $('#fq4').is(':checked');
    config.fq5 = $('#fq5').is(':checked');
    config.fq6 = $('#fq6').is(':checked');
    config.fq7 = $('#fq7').is(':checked');
    config.fq8 = $('#fq8').is(':checked');
    config.mqttF1 = $('#mqttF1').val();
    config.mqttF2 = $('#mqttF2').val();
    config.mqttF3 = $('#mqttF3').val();
    config.mqttF4 = $('#mqttF4').val();
    config.mqttF5 = $('#mqttF5').val();
    config.mqttF6 = $('#mqttF6').val();
    config.mqttF7 = $('#mqttF7').val();
    config.mqttF8 = $('#mqttF8').val();
    config.fd1[0] = Number($('#fd1_s').val());
    config.fd2[0] = Number($('#fd2_s').val());
    config.fd3[0] = Number($('#fd3_s').val());
    config.fd5[0] = Number($('#fd5_s').val());
    config.fd4[0] = Number($('#fd4_s').val());
    config.fd6[0] = Number($('#fd6_s').val());
    config.fd7[0] = Number($('#fd7_s').val());
    config.fd8[0] = Number($('#fd8_s').val());
    config.fd1[1] = Number($('#fd1_t').val());
    config.fd2[1] = Number($('#fd2_t').val());
    config.fd3[1] = Number($('#fd3_t').val());
    config.fd4[1] = Number($('#fd4_t').val());
    config.fd5[1] = Number($('#fd5_t').val());
    config.fd6[1] = Number($('#fd6_t').val());
    config.fd7[1] = Number($('#fd7_t').val());
    config.fd8[1] = Number($('#fd8_t').val());
    config.thngsend = $('#thngsend').is(':checked');
    config.chid = Number($('#chid').val());
    config.wrkey = $('#wrkey').val();
    config.rdkey = $('#rdkey').val();
    config.f1[0] = Number($('#f1_s').val());
    config.f2[0] = Number($('#f2_s').val());
    config.f3[0] = Number($('#f3_s').val());
    config.f5[0] = Number($('#f5_s').val());
    config.f4[0] = Number($('#f4_s').val());
    config.f6[0] = Number($('#f6_s').val());
    config.f7[0] = Number($('#f7_s').val());
    config.f8[0] = Number($('#f8_s').val());
    config.f1[1] = Number($('#f1_t').val());
    config.f2[1] = Number($('#f2_t').val());
    config.f3[1] = Number($('#f3_t').val());
    config.f4[1] = Number($('#f4_t').val());
    config.f5[1] = Number($('#f5_t').val());
    config.f6[1] = Number($('#f6_t').val());
    config.f7[1] = Number($('#f7_t').val());
    config.f8[1] = Number($('#f8_t').val());
    config.nrdmsend = $('#nrdmsend').is(':checked');
    config.lat = $('#lat').val();
    config.lon = $('#lon').val();
    config.nrd_tout = Number($('#nrd_tout').val());
    config.nrd_hout = Number($('#nrd_hout').val());
    config.nrd_pout = Number($('#nrd_pout').val());
    config.nrd_tin = Number($('#nrd_tin').val());
    config.nrd_hin = Number($('#nrd_hin').val());
    config.nrd_lin = Number($('#nrd_lin').val());
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
});
