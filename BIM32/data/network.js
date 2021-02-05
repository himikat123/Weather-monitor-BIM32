let ant = {}, config = {};

function antena(){
  let an = {}, a = {}, c;
  for(c=1; c<8; c++){
    let i = document.getElementById("cnv");
    var cv = document.createElement("canvas");
    if(c < 6) cv.setAttribute("id", "ant" + c);
    if(c == 6) cv.setAttribute("id", "arr");
    if(c == 7) cv.setAttribute("id", "sav");
    i.appendChild(cv);
  }
  for(c=1; c<6; c++){
    an[c] = document.getElementById("ant" + c);
    a[c] = an[c].getContext("2d");
    a[c].fillStyle = "#095";
    for(var x0=0, x1=0, y0=16, y1=2; x0<17; x0+=4, x1++, y0-=2, y1+=2){
      if(c == x1) a[c].fillStyle = "#ccc";
      a[c].fillRect(x0, y0, 3, y1);
    }
    ant[c] = an[c].toDataURL();
  }
}

function ssid_sel(name){
  $('#ssid').val(name);
  $('#ssids').hide();
}

function logout(){
  $.ajax({
    type:"POST",
    url:"esp/logout.php",
    cache:false
  }).done();
  document.cookie='auth=0';
  window.location='http://radiokot.ru';
}

function ip_type(t){
  if(t == "static"){
    $('#ip_s').addClass('hid');
    $('#mask_s').addClass('hid');
    $('#gw_s').addClass('hid');
    $('#dns1_s').addClass('hid');
    $('#dns2_s').addClass('hid');
    $('#ip').removeClass('hid');
    $('#mask').removeClass('hid');
    $('#gw').removeClass('hid');
    $('#dns1').removeClass('hid');
    $('#dns2').removeClass('hid');
  }
  else{
    $('#ip_s').removeClass('hid');
    $('#mask_s').removeClass('hid');
    $('#gw_s').removeClass('hid');
    $('#dns1_s').removeClass('hid');
    $('#dns2_s').removeClass('hid');
    $('#ip').addClass('hid');
    $('#mask').addClass('hid');
    $('#gw').addClass('hid');
    $('#dns1').addClass('hid');
    $('#dns2').addClass('hid');
  }
}

$(function(){
  antena();

  $('#sm').click(function(){
    $('#ssids').toggle();
  });

  $(document).click(function(e){
    let div = $("#ssids");
		if(!$("#sm").is(e.target) && !div.is(e.target) && div.has(e.target).length === 0){
			div.hide();
		}

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
    $(`input:radio[value=${config.type}]`)[0].checked = true;
    ip_type(config.type ? 'static' : 'dynamic');
  });

  setInterval(function(){
    $.ajax({
	    url: 'esp/ssids.php',
	    method: 'get',
	    dataType: 'json',
	    success: function(data){
        $("#ssids").empty();
        let an = 0;
        let entries = Object.entries(data);
        data = entries.sort((a, b) => a[1] - b[1]);
        for(let b in data){
          if(data[b][1] > 102) an = ant[0];
          if(data[b][1] > 87 && data[b][1] < 103) an = ant[1];
          if(data[b][1] > 72 && data[b][1] < 88) an = ant[2];
          if(data[b][1] > 57 && data[b][1] < 73) an = ant[3];
          if(data[b][1] <= 57) an = ant[4];
          $('#ssids').append(`
            <div style="background-image:url('${an}')" onclick="ssid_sel('${data[b][0]}')">
              ${data[b][0]}
            </div>
          `);
        }
        $('#loading').removeClass('active');
	    }
    });
  },5000);

  $('#eye').click(function(){
	  if($('#pass').attr('type') == 'password'){
		  $(this).addClass('view');
		  $('#pass').attr('type', 'text');
	  }
    else{
		  $(this).removeClass('view');
		  $('#pass').attr('type', 'password');
	  }
	  return false;
  });

  $('input[name="ip"]').click(function(){
	  ip_type(this.id);
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    config.ssid = $('#ssid').val();
    config.pass = $('#pass').val();
    config.ip   = $('#ip').val();
    config.mask = $('#mask').val();
    config.gw   = $('#gw').val();
    config.dns1 = $('#dns1').val();
    config.dns2 = $('#dns2').val();
    config.type = ($('input[name=ip]:checked').val() == "true") ? true : false;
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

  $.getJSON("esp/ip_gw.php",function(json){
    for(var b in json){
      try{
        $(`#${b}`).val(json[b]);
      }
      catch(b){}
    }
  });

});
