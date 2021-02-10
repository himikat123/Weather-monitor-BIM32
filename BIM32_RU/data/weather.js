function prov(p){
  if(p == 0){
    $('#appid').removeAttr('disabled');
    $('#appkey').attr('disabled', true);
  }
  if(p == 1){
    $('#appid').attr('disabled', true);
    $('#appkey').removeAttr('disabled');
  }
}

function city_ident(c){
  if(c == 0){
    $('#city').removeAttr('disabled');
    $('#cityid').attr('disabled', true);
    $('#lat').attr('disabled', true);
    $('#lon').attr('disabled', true);
  }
  if(c == 1){
    $('#city').attr('disabled', true);
    $('#cityid').attr('disabled', true);
    $('#lat').removeAttr('disabled');
    $('#lon').removeAttr('disabled');
  }
  if(c == 2){
    $('#city').attr('disabled', true);
    $('#cityid').removeAttr('disabled');
    $('#lat').attr('disabled', true);
    $('#lon').attr('disabled', true);
  }
}

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
    $(`input[name="provider"]`)[config.provider].checked = true;
    $(`input[name="citysearch"]`)[config.citysearch].checked = true;
    prov(config.provider);
    city_ident(config.citysearch);
    $('#loading').removeClass('active');
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    config.lang = 'ru';
    config.appid = $('#appid').val();
    config.appkey = $('#appkey').val();
    config.city = $('#city').val();
    config.cityid = $('#cityid').val();
    config.lat = $('#lat').val();
    config.lon = $('#lon').val();
    config.provider = Number($('input[name=provider]:checked').val());
    config.citysearch = Number($('input[name=citysearch]:checked').val());
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

  $('input[name="provider"]').click(function(){
    prov(this.value);
  });

  $('input[name="citysearch"]').click(function(){
    city_ident(this.value);
  });
});
