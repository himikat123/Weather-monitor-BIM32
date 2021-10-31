function inner(h,e){document.getElementById(h).innerHTML=e}
function get(g){return document.getElementById(g)}

function isTextFile(a){
  var b=/(?:\.([^.]+))?$/.exec(a)[1];
  if(typeof b!==undefined){
    switch(b){
      case "htm":
      case "html":
      case "json":
      case "js":
      case "ino":
      case "php":
      case "css":
        return true;
    }
  }
  return false
}

function isImageFile(a){
  var b=/(?:\.([^.]+))?$/.exec(a)[1];
  if(typeof b!==undefined){
    switch(b){
      case "png":
      case "jpg":
      case "gif":
        return true;
    }
  }
  return false;
}

function ajax(url,opts){
  var req;
  if(window.XMLHttpRequest) req=new XMLHttpRequest();
  else req=new ActiveXObject("Microsoft.XMLHTTP");
  req.onreadystatechange=function(){
    if(req.readyState==4){
      if(/^2/.test(req.status) && opts.success) opts.success(req);
      else if(/^5/.test(req.status) && opts.fail) opts.fail(req);
      else if(opts.other) opts.other(req);
    }
  };
  req.open(opts.method||'POST',url,true);
  req.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
  req.send(opts.data);
}

function select(s){
  hideMenu();
  var sl=document.getElementsByClassName("sl")[0];
  if(sl!=undefined){
    var s0=get(sl.id+"$0");
    if(s0.children.length>0){
      var s1=sl.getElementsByTagName("input")[0].value;
      if(sl.id!=s) s0.innerHTML=s1;
      if(s==0) s0.innerHTML=sl.id;
      if(s==1) s0.innerHTML=s1;
      if(s!=0) r_name(sl.id,s1);
    }
    if(s!=0) sl.classList.remove("sl");
  }
  var ss=get(s);
  var dn=get("dn");
  var dn_mn=get("dn_mn");
  var dl=get("dl");
  var dl_mn=get("dl_mn");
  var rn=get("rn");
  var rn_mn=get("rn_mn");
  if(s!=0 && s!=1){
    ss.classList.add('sl');
    if(ss.classList.contains('dir')){
      dn.classList.add('dis');
      dn_mn.classList.add('disabled');
      dl.classList.add('dis');
      dl_mn.classList.add('disabled');
      rn.classList.add('dis');
      rn_mn.classList.add('disabled');
    }
    else{
      dn.classList.remove("dis");
      dn_mn.classList.remove("disabled");
      dl.classList.remove("dis");
      dl_mn.classList.remove("disabled");
      rn.classList.remove("dis");
      rn_mn.classList.remove("disabled");
    }
    get("op").classList.remove("dis");
    get("op_mn").classList.remove("disabled");
  }
}

function del(){
  var sl=document.getElementsByClassName("sl")[0];
  if(sl!=undefined){
    sl=document.querySelector(".sl").getAttribute("id");
    if(confirm(`Вы уверены что хотите удалить "${sl}" ?`)){
      get('loading').classList.add('active');
      ajax(`esp/del.php?d=${sl}`,{
        method:'GET',
        success:function(r){
          if(r.responseText!="OK") alert(r.responseText);
          list();
        }
      });
    }
  }
}
function down(){
  var sl=document.getElementsByClassName("sl")[0];
  if(sl!=undefined){
    sl=document.querySelector(".sl").getAttribute("id");
    var link=document.createElement('a');
    link.setAttribute('href',sl);
    link.setAttribute('download',sl);
    onload=link.click();
  }
}
function rename(){
  var sl=document.getElementsByClassName("sl")[0];
  if(sl!=undefined){
    sl=document.querySelector(".sl").getAttribute("id");
    var s=document.getElementById(sl+"$0");
    s.innerHTML="<input id="+sl+"$1 value="+sl+"></input>";
    var f=document.getElementById(sl+"$1")
    f.focus();
    f.selectionStart=f.value.length;
    s.onkeydown=function(e){
      if(e.keyCode==27){
        select(0);
        return false;
      }
      if(e.keyCode==13){
        select(1);
        r_name();
        return false;
      }
    }
  }
}
function r_name(s0,s1){
  if(s0!=s1){
    get('loading').classList.add('active');
    ajax(`esp/rename.php?old=${s0}&new=${s1}&DATA=${Math.random()}`,{
      method:'GET',
      success:function(r){
        var d=r.responseText;
        if(d!="OK") alert(d);
      }
    });
    list();
  }
}

function viewer(wv){
  wv.style.display="block";
}

function opn(s){
  var sl=document.getElementsByClassName("sl")[0];
  if(sl!=undefined){
    get('loading').classList.add('active');
    sl=document.querySelector(".sl").getAttribute("id");
    var wv=get("view");
    if(isImageFile(sl)){
      viewer(wv);
      var img=new Image();
      img.onload=function(){
        wv.appendChild(this);
      }
      img.src=sl;
      img.id="pic";
      var v=wv.appendChild(document.createElement('div'));
      v.classList.add('un');
      v.appendChild(document.createTextNode(sl));
      get('loading').classList.remove('active');
    }
    else if(isTextFile(sl)){
      viewer(wv);
      ajax(`${sl}?DATA=${Math.random()}`,{
        method:'GET',
        success:function(r){
          var w=wv.appendChild(document.createElement('div'));
          w.classList.add('un');
          w.appendChild(document.createTextNode(sl));
          var html=r.responseText;
          var v=wv.appendChild(document.createElement('pre'));
          v.appendChild(document.createTextNode(html));
          get('loading').classList.remove('active');
        }
      });
    }
    else{
      viewer(wv);
      var v=wv.appendChild(document.createElement('div'));
      v.classList.add('un');
      v.appendChild(document.createTextNode("неподдерживаемый формат"));
      get('loading').classList.remove('active');
    }
  }
}

function cls(){
  var view=get('view');
  try{view.querySelectorAll('pre')[0].remove();}
  catch(e){}
  try{view.querySelectorAll('div')[0].remove();}
  catch(e){}
  try{view.querySelectorAll('img')[0].remove();}
  catch(e){}
  view.style.display="none";
}

function list(){
  ajax(`/esp/list.php?DATA=${Math.random()}`,{
    method:'GET',
    success:function(r){
      var d,data=JSON.parse(r.responseText);
      var r="<tr><th>Имя</th><th>Тип</th><th>Размер</th></tr>";
      var tbl=get("tb");
      data.fl.sort(function(a,b){
        if(a.name<b.name){return -1;}
        if(a.name>b.name){return 1;}
        return 0;
      });
      for(d in data.fl){
        var name=data.fl[d].name.slice(1);
        r+=`<tr id="${name}" onmousedown="select('${name}')" ondblclick="opn()" class="${data.fl[d].type}">`;
        r+=`<td><img src="${(isTextFile(name))?"textfile.png":(isImageFile(name))?"imagefile.png":"unknown.png"}"><span id="${name}$0">${name}</span></td>`;
        r+=`<td class="td1">Файл "${name.split(".")[1].toUpperCase()}"</td>`;
        r+=`<td class="td2">${(Math.round((data.fl[d].size/1024)*100)/100)}кБ</td></tr>`;
        tbl.innerHTML=r;
      }
      for(var b in data.fs){
        try{get(b).innerHTML=data.fs[b]}
        catch(b){}
      }
      get('loading').classList.remove('active');
    }
  });
}

function loaded(){
  document.addEventListener('click',function(e){
    if(e.target.id!='submenu'){
      hideMenu();
    }
    if(e.target.id!='navmenu'&&e.target.id!='nav-toggle'&&e.target.id!='mn'){
      get('nav-toggle').checked=false;
    }
  });

  var h,d="uploader";
  var i=document.createElement("input");
  i.type="file";
  i.multiple=false;
  i.name="data";
  i.className="but upl";
  get(d).appendChild(i);
  var b=document.createElement("br");
  get(d).appendChild(b);
  var j=document.createElement("input");
  j.id="upload-path";
  j.type="text";
  j.name="path";
  j.className="but";
  j.defaultValue="/";
  get(d).appendChild(j);
  var k=document.createElement("button");
  k.innerHTML='Загрузить';
  k.className="but";
  get(d).appendChild(k);

  function httpPostProcessRequest(){
    get('loading').classList.add('active');
    if(h.readyState==4){
      if(h.status!=200){
        alert(`ERROR["${h.status}"]: ${h.responseText}`);
        list();
      }
      else list();
    }
  }

  function createPath(p){
    h=new XMLHttpRequest();
    h.onreadystatechange=httpPostProcessRequest;
    var a=new FormData();
    a.append("path",p);
    h.open("PUT","/edit");
    h.send(a)
  }

  k.onclick=function(e){
    if(i.files.length===0){
      return
    }
    h=new XMLHttpRequest();
    h.onreadystatechange=httpPostProcessRequest;
    var a=new FormData();
    a.append("data",i.files[0],j.value);
    h.open("POST","/edit");
    h.send(a)
  };

  i.onchange=function(e){
    if(i.files.length===0) return;
    var a=i.files[0].name;
    var b=/(?:\.([^.]+))?$/.exec(a)[1];
    var c=/(.*)\.[^.]+$/.exec(a)[1];
    if(typeof c!==undefined){
      a=c
    }
    if(typeof b!==undefined){
      if(b==="html") b="htm";
      else if(b==="jpeg") b="jpg";
      a=a+"."+b
    }
    var p=a;
    j.value="/"+p;
  }
  list();
}

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}
