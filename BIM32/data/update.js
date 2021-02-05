function loaded(page){
  document.addEventListener('click',function(e){
    if(e.target.id!='navmenu'&&e.target.id!='nav-toggle'&&e.target.id!='mn'){
      document.getElementById('nav-toggle').checked=false;
    }
  });

  var altpr='p0';
  for(var i=1;i<101;i++){
    $('#stl').append('.progress-circle.p'+i+' .value-bar {transform:rotate('+Math.round(360*i/100)+'deg)}');
  }
  $('#fl').change(function(e){
    $('#fnm').text(e.target.files[0].name);
  });
  $('form').submit(function(e){
    e.preventDefault();
    var form=$('#upload_form')[0];
    var data=new FormData(form);
    $.ajax({
      url:'/doUpdate',
      type:'POST',
      data:data,
      contentType:false,
      processData:false,
      xhr:function(){
        var xhr=new window.XMLHttpRequest();
        xhr.upload.addEventListener('progress',function(evt){
          if(evt.lengthComputable){
            var per=evt.loaded/evt.total;
            var prc=Math.round(per*100);
            $('#prg').html(prc+'%');
            if(prc>=50) $('#kr').addClass('over50');
            else $('#kr').removeClass('over50');
            $('#kr').addClass('p'+prc).removeClass(altpr);
            altpr='p'+prc;
          }
        },false);
        return xhr;
      },
      success:function(d,s){
        console.log('success!');
        window.top.location="login.htm";
      },
      error:function(a,b,c){}
    });
  });
  $('#loading').removeClass('active');
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
