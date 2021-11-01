let config, sig, dbmet, dbmeh, dbmep, dbmpt, dbmpp, dshtt, dshth, ddhtt, ddhth;
let updt, wtemp, whum, wpres, wlight, wubat, wlbat, thing;
let git = 'https://github.com/himikat123/Weather-monitor-BIM32/blob/master/BIM32_RU/data/';

function status_update(){
  $.getJSON("esp/status.php",function(json){
    $('#fw').html(json['fw']);
    $('#int_temp').html(json['int_temp']);
    sig = json['sig'];
    dbmet = json['dbmet'];
    dbmeh = json['dbmeh'];
    dbmep = json['dbmep'];
    dbmpt = json['dbmpt'];
    dbmpp = json['dbmpp'];
    dshtt = json['dshtt'];
    dshth = json['dshth'];
    ddhtt = json['ddhtt'];
    ddhth = json['ddhth'];
    wtemp = json['temp_wsens'];
    whum = json['hum_wsens'];
    wpres = json['pres_wsens'];
    wlight = json['light_wsens'];
    wlbat = json['wbat_level'];
    wubat = json['wbat_voltage'];
    $('#loading').removeClass('active');
  });
}

function logout(){
  document.cookie='auth=0';
  window.location='login.htm';
}

function weatherbit_icon(code){
  let icon = 4;
  if(code >= 200 && code < 300) icon = 11;
  if(code >= 300 && code < 400) icon = 9;
  if(code >= 500 && code < 600) icon = 10;
  if(code >= 600 && code < 700) icon = 13;
  if(code >= 700 && code < 800) icon = 50;
  if(code == 800) icon = 1;
  if(code == 801 || code == 802) icon = 2;
  if(code == 803) icon = 3;
  if(code >= 804) icon = 4;
  return icon;
}

function clock_upd(context){
  let days = ['Ni', 'Pn', 'Wt', 'Śr', 'Cz', 'Pt', 'So'];
  let now = new Date();
  let hr0 = Math.floor(now.getHours() / 10);
  let hr1 = now.getHours() % 10;
  let mn0 = Math.floor(now.getMinutes() / 10);
  let mn1 = now.getMinutes() % 10;
  let wd  = now.getDay();
  if(hr0 == 0) hr0 = 'dig';
  let clock_hr0 = new Image();
  clock_hr0.onload = function(){
    context.drawImage(clock_hr0, 2, 2);
  };
  clock_hr0.src = `${git}${hr0}.png?raw=true`;
  let clock_hr1 = new Image();
  clock_hr1.onload = function(){
    context.drawImage(clock_hr1, 48, 2);
  };
  clock_hr1.src = `${git}${hr1}.png?raw=true`;
  let clock_mn0 = new Image();
  clock_mn0.onload = function(){
    context.drawImage(clock_mn0, 116, 2);
  };
  clock_mn0.src = `${git}${mn0}.png?raw=true`;
  let clock_mn1 = new Image();
  clock_mn1.onload = function(){
    context.drawImage(clock_mn1, 162, 2);
  };
  clock_mn1.src = `${git}${mn1}.png?raw=true`;

  context.fillStyle = "black";
  context.fillRect(211, 0, 60, 57);
  context.fillRect(12, 225, 40, 24);
  context.fillRect(132, 225, 40, 24);
  context.fillRect(252, 225, 40, 24);
  context.fillRect(372, 225, 40, 24);
  context.font = '48px Ubuntu';
  context.fillStyle = "rgb(0, 255, 0)";
  context.fillText(days[wd], 212, 38);
  context.font = '22px Ubuntu';
  context.fillStyle = "white";
  context.fillText(days[wd], 15, 242);
  wd++;
  if(wd > 6) wd = 0;
  context.fillText(days[wd], 135, 242);
  wd++;
  if(wd > 6) wd = 0;
  context.fillText(days[wd], 255, 242);
  wd++;
  if(wd > 6) wd = 0;
  context.fillText(days[wd], 375, 242);

  let ant = new Image();
  let ant_level = 1;
  let level = Number(String(sig).replace(/[^0-9\.-]+/g,""));
  if(level > -51) ant_level = 4;
  if(level < -50 && level > -76) ant_level = 3;
  if(level < -75 && level > -96) ant_level = 2;
  if(level < -95) ant_level = 1;
  ant.onload = function(){
    context.drawImage(ant, 431, 1);
  };
  ant.src = `${git}ant${ant_level}.png?raw=true`;

  if(updt != undefined){
    context.fillStyle = "black";
    context.fillRect(249, 200, 230, 20);
    context.font = '14px Ubuntu';
    context.fillStyle = "white";
    context.fillText('⭮' + updt, 475 - context.measureText('⭮' + updt).width, 214);
  }
}

function curr_weather(context, icon, w_dir, descript, w_speed, temp_weather, hum_weather, pres_weather){
  let cf = ['Przyjemny', 'Gorąco', 'Zimno', 'Zbyt sucho', 'Zbyt wilgotno',
            'Gorąco i wilgotno', 'Gorąco i sucho', 'Zimno i wilgotno', 'Zimno i sucho'];
  if(icon == '03d') icon = '02d';
  if(icon == '03n') icon = '02n';
  let icon_img = new Image();
  icon_img.onload = function(){
    context.drawImage(icon_img, 0, 116);
  };
  icon_img.src = `${git}${icon}.png?raw=true`;

  let wind_icon = new Image();
  wind_icon.onload = function(){
    context.drawImage(wind_icon, 97, 194);
  };
  wind_icon.src = `${git}wind.png?raw=true`;
  let temp_icon = new Image();
  temp_icon.onload = function(){
    context.drawImage(temp_icon, 99, 140);
  };
  temp_icon.src = `${git}temp.png?raw=true`;

  let wind_dir = new Image();
  wind_dir.onload = function(){
    context.drawImage(wind_dir, 222, 195);
  };
  let deg = Math.round(w_dir / 45.0) + 1;
  if(deg > 7) deg = 1;
  wind_dir.src = `${git}w${deg}.png?raw=true`;

  let temp_out, temp_in, hum_out, hum_in, pres_out, ubat, lbat;
  if(config != undefined){
    switch(Number(config['temp_out'])){
      case 0: temp_out = temp_weather; break;
      case 1: temp_out = wtemp; break;
      case 2: temp_out = dbmet; break;
      case 3: temp_out = dbmpt; break;
      case 4: temp_out = dshtt; break;
      case 5: temp_out = ddhtt; break;
      case 9: temp_out = thing['field' + config.tto]; break;
      default: temp_out = '--'; break;
    }
    switch(Number(config['temp_in'])){
      case 0: temp_in = temp_weather; break;
      case 1: temp_in = wtemp; break;
      case 2: temp_in = dbmet; break;
      case 3: temp_in = dbmpt; break;
      case 4: temp_in = dshtt; break;
      case 5: temp_in = ddhtt; break;
      case 9: temp_in = thing['field' + config.tti]; break;
      default: temp_in = '--'; break;
    }
    switch(Number(config['hum_out'])){
      case 0: hum_out = hum_weather; break;
      case 1: hum_out = whum; break;
      case 2: hum_out = dbmeh; break;
      case 4: hum_out = dshth; break;
      case 5: hum_out = ddhth; break;
      case 9: hum_out = thing['field' + config.tho]; break;
      default: hum_out = '--'; break;
    }
    switch(Number(config['hum_in'])){
      case 0: hum_in = hum_weather; break;
      case 1: hum_in = whum; break;
      case 2: hum_in = dbmeh; break;
      case 4: hum_in = dshth; break;
      case 5: hum_in = ddhth; break;
      case 9: hum_in = thing['field' + config.thi]; break;
      default: hum_in = '--'; break;
    }
    switch(Number(config['pres_out'])){
      case 0: pres_out = pres_weather; break;
      case 1: pres_out = wpres; break;
      case 2: pres_out = dbmep; break;
      case 3: pres_out = dbmpp; break;
      case 9: pres_out = thing['field' + config.tpo]; break;
      default: pres_out = '--'; break;
    }
    switch(Number(config['bat_disp'])){
      case 1: {
        ubat = wubat;
        lbat = wlbat;
      } break;
      case 9: {
        ubat = thing['field' + config.tbt];
        if(ubat < (3.6)) lbat = 1;
        else if(ubat < 3.7) lbat = 2;
        else if(ubat < 3.8) lbat = 3;
        else lbat = 4;
      } break;
      default: {
        ubat = undefined;
        lbat = undefined;
      } break;
    }
  }

  temp_out = (temp_out == undefined || temp_out == '--' || temp_out > 99 || temp_out < -49) ? '--' : Math.round(temp_out) + '°';
  temp_in = (temp_in == undefined || temp_in == '--' || temp_in > 99 || temp_in < -49) ? '--' : Math.round(temp_in) + '°';
  hum_out = (hum_out == undefined || hum_out == '--' || hum_out > 100 || hum_out < 0) ? '--' : (hum_out > 99) ? Math.round(hum_out) : Math.round(hum_out) + '%';
  hum_in = (hum_in == undefined || hum_in == '--' || hum_in > 100 || hum_in < 0) ? '--' : Math.round(hum_in) + '%';
  pres_out = (pres_out == undefined || pres_out == '--' || pres_out > 800 || pres_out < 500) ? '--' : Math.round(pres_out) + 'mm';
  ubat = (ubat == undefined || ubat == '--' || ubat > 20 || ubat < 0) ? '' : Math.round(ubat * 10) / 10;
  lbat = (lbat == undefined || lbat == '--' || lbat > 4 || lbat < 1) ? 0 : lbat;

  context.fillStyle = "black";
  context.fillRect(98, 109, 381, 29);
  context.fillRect(167, 195, 50, 25);
  context.fillRect(260, 60, 100, 45);
  context.fillRect(402, 60, 78, 45);
  context.fillRect(116, 143, 100, 48);
  context.fillRect(262, 143, 58, 48);
  context.fillRect(372, 143, 98, 48);
  context.fillRect(340, 12, 30, 16);
  context.fillRect(272, 35, 157, 22);
  context.font = '40px Ubuntu';
  context.fillStyle = "rgb(255, 255, 0)";
  context.fillText(temp_out, 160 - context.measureText(temp_out).width / 2, 185);
  context.fillText(temp_in, 310 - context.measureText(temp_in).width / 2, 100);
  context.font = '28px Ubuntu';
  context.fillStyle = "rgb(0, 255, 255)";
  context.fillText(hum_out, 292 - context.measureText(hum_out).width / 2, 188);
  context.fillText(hum_in, 435 - context.measureText(hum_in).width / 2, 100);
  context.fillStyle = "rgb(255, 0, 255)";
  context.fillText(pres_out, 420 - context.measureText(pres_out).width / 2, 188);
  context.fillStyle = "white";
  context.fillText(descript, 288 - context.measureText(descript).width / 2, 131);
  context.font = '20px Ubuntu';
  context.fillText(`${w_speed}m/s`, 167, 215);
  let bat = new Image();
  bat.onload = function(){
    context.drawImage(bat, 377, 1);
  };
  bat.src = `${git}bat${lbat}.png?raw=true`;
  context.fillStyle = "rgb(0, 255, 0)";
  context.font = '14px Ubuntu';
  if(ubat != '') context.fillText(`${ubat}в`, 340, 26);

  context.fillStyle = "white";
  context.font = '16px Ubuntu';
  if(temp_in != '--' && hum_in != '--'){
    let cfrt = 0;
    let t_in = parseInt(temp_in.replace(/[^\d]/g, ''));
    let h_in = parseInt(hum_in.replace(/[^\d]/g, ''));
    if(t_in >= 22.0 && t_in <= 27.0 && h_in >= 30 && h_in <= 60) cfrt = 0;
    if(t_in > 27.0 && h_in >= 30 && h_in <= 60) cfrt = 1;
    if(t_in < 22.0 && h_in >= 30 && h_in <= 60) cfrt = 2;
    if(t_in >= 22.0 && t_in <= 27.0 && h_in < 30) cfrt = 3;
    if(t_in >= 22.0 && t_in <= 27.0 && h_in > 60) cfrt = 4;
    if(t_in > 27.0 && h_in > 60) cfrt = 5;
    if(t_in > 27.0 && h_in < 30) cfrt = 6;
    if(t_in < 22.0 && h_in > 60) cfrt = 7;
    if(t_in < 22.0 && h_in < 30) cfrt = 8;
    context.fillText(cf[cfrt], 350 - context.measureText(cf[cfrt]).width / 2, 50);
  }
}

function forecast(context, icon_daily, temp_max_daily, temp_min_daily, w_speed_daily){
  if(icon_daily[0] == 3) icon_daily[0] = 2;
  let icon1_img = new Image();
  icon1_img.onload = function(){
    context.drawImage(icon1_img, 3, 247);
  };
  icon1_img.src = `${git}i${icon_daily[0]}.png?raw=true`;
  if(icon_daily[1] == 3) icon_daily[1] = 2;
  let icon2_img = new Image();
  icon2_img.onload = function(){
    context.drawImage(icon2_img, 123, 247);
  };
  icon2_img.src = `${git}i${icon_daily[1]}.png?raw=true`;
  if(icon_daily[2] == 3) icon_daily[2] = 2;
  let icon3_img = new Image();
  icon3_img.onload = function(){
    context.drawImage(icon3_img, 243, 247);
  };
  icon3_img.src = `${git}i${icon_daily[2]}.png?raw=true`;
  if(icon_daily[3] == 3) icon_daily[3] = 2;
  let icon4_img = new Image();
  icon4_img.onload = function(){
    context.drawImage(icon4_img, 363, 247);
  };
  icon4_img.src = `${git}i${icon_daily[3]}.png?raw=true`;

  context.fillStyle = "black";
  context.fillRect(52, 236, 66, 28);
  context.fillRect(172, 236, 66, 28);
  context.fillRect(292, 236, 66, 28);
  context.fillRect(412, 236, 66, 28);
  context.fillRect(52, 263, 66, 28);
  context.fillRect(172, 263, 66, 28);
  context.fillRect(292, 263, 66, 28);
  context.fillRect(412, 263, 66, 28);
  context.fillRect(20, 296, 80, 23);
  context.fillRect(140, 296, 80, 23);
  context.fillRect(260, 296, 80, 23);
  context.fillRect(380, 296, 80, 23);
  context.font = '26px Ubuntu';
  context.fillStyle = "rgb(255, 255, 0)";
  context.fillText(`${temp_max_daily[0]}°`, 86 - context.measureText(`${temp_max_daily[0]}°`).width / 2, 262);
  context.fillText(`${temp_max_daily[1]}°`, 206 - context.measureText(`${temp_max_daily[1]}°`).width / 2, 262);
  context.fillText(`${temp_max_daily[2]}°`, 326 - context.measureText(`${temp_max_daily[2]}°`).width / 2, 262);
  context.fillText(`${temp_max_daily[3]}°`, 446 - context.measureText(`${temp_max_daily[3]}°`).width / 2, 262);
  context.fillStyle = "rgb(255, 127, 0)";
  context.fillText(`${temp_min_daily[0]}°`, 86 - context.measureText(`${temp_min_daily[0]}°`).width / 2, 289);
  context.fillText(`${temp_min_daily[1]}°`, 206 - context.measureText(`${temp_min_daily[1]}°`).width / 2, 289);
  context.fillText(`${temp_min_daily[2]}°`, 326 - context.measureText(`${temp_min_daily[2]}°`).width / 2, 289);
  context.fillText(`${temp_min_daily[3]}°`, 446 - context.measureText(`${temp_min_daily[3]}°`).width / 2, 289);
  context.font = '22px Ubuntu';
  context.fillStyle = "white";
  context.fillText(`${w_speed_daily[0]}m/s`, 63 - context.measureText(`${w_speed_daily[0]}m/s`).width / 2, 314);
  context.fillText(`${w_speed_daily[1]}m/s`, 183 - context.measureText(`${w_speed_daily[1]}m/s`).width / 2, 314);
  context.fillText(`${w_speed_daily[2]}m/s`, 303 - context.measureText(`${w_speed_daily[2]}m/s`).width / 2, 314);
  context.fillText(`${w_speed_daily[3]}m/s`, 423 - context.measureText(`${w_speed_daily[3]}m/s`).width / 2, 314);
}

function weather(){
  let canvas = document.getElementById("display_canvas");
  let context = canvas.getContext("2d");
  context.rect(0, 0, 480, 320);
  context.fillStyle = 'black';
  context.fill();
  let temp_max_daily = [-50, -50, -50, -50],
      temp_min_daily = [50, 50, 50, 50],
      icon_daily = [1, 1, 1, 1],
      w_speed_daily = [0, 0, 0, 0],
      descript, icon, w_speed, w_dir, temp_weather, hum_weather, pres_weather;

  $.getJSON(`config.json?R=${Math.random()}`,function(json){
    config = json;
    for(var b in config){
      try{$(`#${b}`).val(config[b]);}
      catch(b){}
    }

    let ground = new Image();
    ground.onload = function(){
      context.drawImage(ground, 0, 0);
    };
    ground.src = `${git}disp.png?raw=true`;

    setInterval(function(){
      clock_upd(context);
    }, 1000);

    let current = '', hourly = '', citysearch = '';
    if(config.provider == 0){
      current = "https://api.openweathermap.org/data/2.5/weather";
      hourly = "https://api.openweathermap.org/data/2.5/forecast";
      if(config.citysearch == 0) citysearch = "?q=" + config.city;
      if(config.citysearch == 1) citysearch = "?lat=" + config.lat + "&lon=" + config.lon;
      if(config.citysearch == 2) citysearch = "?id=" + String(config.cityid);
      current += `${citysearch}&units=metric&appid=${config.appid}&lang=${config.lang}`;
      hourly += `${citysearch}&units=metric&appid=${config.appid}`;
    }
    if(config.provider == 1){
      current = `https://api.weatherbit.io/v2.0/current?key=${config.appkey}`;
      hourly = `https://api.weatherbit.io/v2.0/forecast/daily?key=${config.appkey}`;
      if(config.citysearch == 0) citysearch = "&city=" + config.city;
      if(config.citysearch == 1) citysearch = "&lat=" + config.lat + "&lon=" + config.lon;
      if(config.citysearch == 2) citysearch = "&city_id=" + config.cityid;
      current += `${citysearch}&lang=${config.lang}`;
      hourly += `${citysearch}&days=4`;
    }

    $.getJSON(current, function(json){
      if(config.provider == 0){
        temp_weather = Math.round(json["main"]["temp"]);
        hum_weather = json["main"]["humidity"];
        pres_weather = Math.round(json["main"]["pressure"] * 0.75);
        descript = json["weather"][0]["description"];
        icon = json["weather"][0]["icon"];
        w_speed = Math.round(json["wind"]["speed"]);
        w_dir = json["wind"]["deg"];
      }
      if(config.provider == 1){
        temp_weather = Math.round(json["data"][0]["temp"]);
        hum_weather = json["data"][0]["rh"];
        pres_weather = Math.round(json["data"][0]["pres"] * 0.75);
        descript = json["data"][0]["weather"]["description"];
        let pod = json["data"][0]["pod"];
        let icn = weatherbit_icon(json["data"][0]["weather"]["code"]);
        if(icn < 10) icon = '0' + icn + pod;
        else icon = icn + pod;
        w_speed = Math.round(json["data"][0]["wind_spd"]);
        w_dir = json["data"][0]["wind_dir"];
      }
      setInterval(function(){
        curr_weather(context, icon, w_dir, descript, w_speed, temp_weather, hum_weather, pres_weather);
      }, 1000);
    });
    //////////////////////////////////////
    $.getJSON(hourly, function(json){
      let date_hourly = [], temp_hourly = [], icon_hourly = [], wind_hourly = [];
      if(config.provider == 0){
        for(let i=0; i<40; i++){
          date_hourly[i] = Date.parse(json['list'][i]['dt_txt']);
          icon_hourly[i] = parseInt(json['list'][i]['weather'][0]['icon'].replace(/[^\d]/g, ''));
          temp_hourly[i] = Math.round(json['list'][i]['main']['temp']);
          wind_hourly[i] = Math.round(json['list'][i]['wind']['speed']);
          let now_dt = new Date();
          let wd = now_dt.getDay();
          let i_dt = new Date(date_hourly[i]);
          let hr = i_dt.getHours();
          let i_wd = i_dt.getDay();
          if(i == 0){
            temp_max_daily[0] = temp_hourly[0];
            temp_min_daily[0] = temp_hourly[0];
            icon_daily[0] = icon_hourly[0];
            w_speed_daily[0] = wind_hourly[0];
          }
          if(i_wd == wd){
            if(hr == 0 || temp_max_daily[0] < temp_hourly[i]) temp_max_daily[0] = temp_hourly[i];
            if(hr == 0 || temp_min_daily[0] > temp_hourly[i]) temp_min_daily[0] = temp_hourly[i];
            if(hr == 0 || icon_daily[0] < icon_hourly[i]) icon_daily[0] = icon_hourly[i];
            if(hr == 0 || w_speed_daily[0] < wind_hourly[i]) w_speed_daily[0] = wind_hourly[i];
          }
          wd++;
          if(wd > 6) wd = 0;
          if(i_wd == wd){
            if(hr == 0 || temp_max_daily[1] < temp_hourly[i]) temp_max_daily[1] = temp_hourly[i];
            if(hr == 0 || temp_min_daily[1] > temp_hourly[i]) temp_min_daily[1] = temp_hourly[i];
            if(hr == 0 || icon_daily[1] < icon_hourly[i]) icon_daily[1] = icon_hourly[i];
            if(hr == 0 || w_speed_daily[1] < wind_hourly[i]) w_speed_daily[1] = wind_hourly[i];
          }
          wd++;
          if(wd > 6) wd = 0;
          if(i_wd == wd){
            if(hr == 0 || temp_max_daily[2] < temp_hourly[i]) temp_max_daily[2] = temp_hourly[i];
            if(hr == 0 || temp_min_daily[2] > temp_hourly[i]) temp_min_daily[2] = temp_hourly[i];
            if(hr == 0 || icon_daily[2] < icon_hourly[i]) icon_daily[2] = icon_hourly[i];
            if(hr == 0 || w_speed_daily[2] < wind_hourly[i]) w_speed_daily[2] = wind_hourly[i];
          }
          wd++;
          if(wd > 6) wd = 0;
          if(i_wd == wd){
            if(hr == 0 || temp_max_daily[3] < temp_hourly[i]) temp_max_daily[3] = temp_hourly[i];
            if(hr == 0 || temp_min_daily[3] > temp_hourly[i]) temp_min_daily[3] = temp_hourly[i];
            if(hr == 0 || icon_daily[3] < icon_hourly[i]) icon_daily[3] = icon_hourly[i];
            if(hr == 0 || w_speed_daily[3] < wind_hourly[i]) w_speed_daily[3] = wind_hourly[i];
          }
        }
      }

      if(config.provider == 1){
        temp_max_daily[0] = Math.round(json["data"][0]["high_temp"]);
        temp_min_daily[0] = Math.round(json["data"][0]["min_temp"]);
        icon_daily[0] = weatherbit_icon(json["data"][0]["weather"]["code"]);
        w_speed_daily[0] = Math.round(json["data"][0]["wind_spd"]);
        temp_max_daily[1] = Math.round(json["data"][1]["high_temp"]);
        temp_min_daily[1] = Math.round(json["data"][1]["min_temp"]);
        icon_daily[1] = weatherbit_icon(json["data"][1]["weather"]["code"]);
        w_speed_daily[1] = Math.round(json["data"][1]["wind_spd"]);
        temp_max_daily[2] = Math.round(json["data"][2]["high_temp"]);
        temp_min_daily[2] = Math.round(json["data"][2]["min_temp"]);
        icon_daily[2] = weatherbit_icon(json["data"][2]["weather"]["code"]);
        w_speed_daily[2] = Math.round(json["data"][2]["wind_spd"]);
        temp_max_daily[3] = Math.round(json["data"][3]["high_temp"]);
        temp_min_daily[3] = Math.round(json["data"][3]["min_temp"]);
        icon_daily[3] = weatherbit_icon(json["data"][3]["weather"]["code"]);
        w_speed_daily[3] = Math.round(json["data"][3]["wind_spd"]);
      }
      let now = new Date();
      let hr = now.getHours();
      let mn = now.getMinutes();
      let sc =now.getSeconds();
      let dt  = now.getDate();
      let mo  = now.getMonth() + 1;
      let yr  = now.getFullYear();
      updt = `${hr}:${('0' + mn).slice(-2)}:${('0' + sc).slice(-2)} ${('0' + dt).slice(-2)}-${('0' + mo).slice(-2)}-${yr}`;

      setInterval(function(){
        forecast(context, icon_daily, temp_max_daily, temp_min_daily, w_speed_daily);
      }, 1000);
    });

    if(config.thngrcv){
      $.getJSON(`https://api.thingspeak.com/channels/${config.chid}/feeds.json?api_key=${config.rdkey}&results=1`, function(json){
        thing = json["feeds"][0];
      });
    }
  });
  return context;
}

$(function(){
  let context = weather();
  setInterval(function(){
    location.reload();
  }, 600000);

  let half_sec = false;
  setInterval(function(){
    context.beginPath();
    if(half_sec) context.fillStyle = "rgb(0, 255, 255)";
    else context.fillStyle = "rgb(0, 0, 0)";
    context.arc(103, 36, 5, 0, 2 * Math.PI, true);
    context.arc(103, 70, 5, 0, 2 * Math.PI, true);
    context.fill();
    half_sec = !half_sec;
  }, 500);

  status_update();
  setInterval(status_update, 5e3);

  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }
  });
});
