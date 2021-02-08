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

  $.getJSON(`esp/username.php?R=${Math.random()}`, function(json){
    $('#user').val(json.user);
    $('#loading').removeClass('active');
  });

  $('#eye1').click(function(){
	  if($('#oldpass').attr('type') == 'password'){
		  $(this).addClass('view');
		  $('#oldpass').attr('type', 'text');
	  }
    else{
		  $(this).removeClass('view');
		  $('#oldpass').attr('type', 'password');
	  }
	  return false;
  });

  $('#eye2').click(function(){
	  if($('#newpass').attr('type') == 'password'){
		  $(this).addClass('view');
		  $('#newpass').attr('type', 'text');
	  }
    else{
		  $(this).removeClass('view');
		  $('#newpass').attr('type', 'password');
	  }
	  return false;
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    let name = $('#user').val();
    let oldpass = $('#oldpass').val();
    let newpass = $('#newpass').val();
    $.ajax({
	    url: 'esp/user.php',
	    method: 'post',
      data: `name=${name}&oldpass=${oldpass}&newpass=${newpass}`,
	    success: function(answ){
        if(answ != "OK") alert("Invalid old password");
        $('#loading').removeClass('active');
	    }
    });
    return false;
  });
});
