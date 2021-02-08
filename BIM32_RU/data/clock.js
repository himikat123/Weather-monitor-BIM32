let config = {};

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
	    }
    });
    return false;
  });
});
