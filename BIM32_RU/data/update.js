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

  $("#disp_up").submit(function(event){
    event.preventDefault();
    var formData = new FormData($(this)[0]);
    $.ajax({
      url: 'esp/dsp.php',
      type: 'POST',
      data: formData,
      async: false,
      cache: false,
      contentType: false,
      processData: false,
      success: function(returndata){
        alert(returndata);
      }
    });
    return false;
  });

  $('#upload_form').submit(function(e){
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
            if(prc>=100){
              setTimeout(function(){
                window.location="login.htm";
              }, 5000);
            }
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
        window.location="login.htm";
      },
      error:function(a,b,c){}
    });
  });
  $('#loading').removeClass('active');
}

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}
