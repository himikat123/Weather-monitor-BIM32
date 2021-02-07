let git = 'https://github.com/himikat123/Weather-monitor-BIM32/blob/master/BIM32/data/';

function bright_type(c){
  if(c == 0){
    $('#auto_day').removeAttr('disabled');
    $('#auto_night').removeAttr('disabled');
    $('#day_time').attr('disabled', true);
    $('#night_time').attr('disabled', true);
    $('#time_day').attr('disabled', true);
    $('#time_night').attr('disabled', true);
    $('#light_in').attr('disabled', true);
    $('#const_day').attr('disabled', true);
  }
  if(c == 1){
    $('#auto_day').attr('disabled', true);
    $('#auto_night').attr('disabled', true);
    $('#day_time').removeAttr('disabled');
    $('#night_time').removeAttr('disabled');
    $('#time_day').removeAttr('disabled');
    $('#time_night').removeAttr('disabled');
    $('#light_in').attr('disabled', true);
    $('#const_day').attr('disabled', true);
  }
  if(c == 2){
    $('#auto_day').attr('disabled', true);
    $('#auto_night').attr('disabled', true);
    $('#day_time').attr('disabled', true);
    $('#night_time').attr('disabled', true);
    $('#time_day').attr('disabled', true);
    $('#time_night').attr('disabled', true);
    $('#light_in').removeAttr('disabled');
    $('#const_day').attr('disabled', true);
  }
  if(c == 3){
    $('#auto_day').attr('disabled', true);
    $('#auto_night').attr('disabled', true);
    $('#day_time').attr('disabled', true);
    $('#night_time').attr('disabled', true);
    $('#time_day').attr('disabled', true);
    $('#time_night').attr('disabled', true);
    $('#light_in').attr('disabled', true);
    $('#const_day').removeAttr('disabled');
  }
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

function rgb16_to_color(c){
  let rgb = '#FFF';
  switch(Number(c)){
    case 31    : rgb = '#00F'; break;
    case 2016  : rgb = '#0F0'; break;
    case 2047  : rgb = '#0FF'; break;
    case 63488 : rgb = '#F00'; break;
    case 63519 : rgb = '#F0F'; break;
    case 65504 : rgb = '#FF0'; break;
    case 65535 : rgb = '#FFF'; break;
    default    : rgb = '#FFF'; break;
  }
  return rgb;
}

function color_to_rgb16(c){
  let rgb = 65535;
  switch(c){
    case 'rgb(0, 0, 255)'     : rgb = 31;    break;
    case 'rgb(0, 255, 0)'     : rgb = 2016;  break;
    case 'rgb(0, 255, 255)'   : rgb = 2047;  break;
    case 'rgb(255, 0, 0)'     : rgb = 63488; break;
    case 'rgb(255, 0, 255)'   : rgb = 63519; break;
    case 'rgb(255, 255, 0)'   : rgb = 65504; break;
    case 'rgb(255, 255, 255)' : rgb = 65535; break;
    default                   : rgb = 65535; break;
  }
  return rgb;
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
  let days = ['Вс', 'Пн', 'Вт', 'Ср', 'Чт', 'Пт', 'Сб'];
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
}

function curr_weather(context, icon, w_dir, descript, w_speed){
  if(icon == '03d') icon = '02d';
  if(icon == '03n') icon = '02n';
  let icon_img = new Image();
  icon_img.onload = function(){
    context.drawImage(icon_img, 0, 116);
  };
  if(icon != undefined) icon_img.src = `${git}${icon}.png?raw=true`;

  let wind_dir = new Image();
  wind_dir.onload = function(){
    context.drawImage(wind_dir, 222, 195);
  };
  let deg = Math.round(w_dir / 45.0) + 1;
  if(deg > 7) deg = 0;
  if(deg > 0 && deg < 100) wind_dir.src = `${git}w${deg}.png?raw=true`;

  context.fillStyle = "black";
  context.fillRect(98, 109, 381, 29);
  context.fillRect(167, 195, 50, 25);
  context.font = '24px Ubuntu';
  context.fillStyle = "white";
  if(descript != undefined) context.fillText(descript, 288 - context.measureText(descript).width / 2, 130);
  context.font = '20px Ubuntu';
  if(w_speed != undefined) context.fillText(`${w_speed}м/с`, 167, 215);
}

function forecast(context, icon_daily, temp_max_daily, temp_min_daily, w_speed_daily){
  if(icon_daily[0] == 3) icon_daily[0] = 2;
  let icon1_img = new Image();
  icon1_img.onload = function(){
    context.drawImage(icon1_img, 3, 247);
  };
  if(icon_daily[0] > 0 && icon_daily[0] <= 50) icon1_img.src = `${git}i${icon_daily[0]}.png?raw=true`;
  if(icon_daily[1] == 3) icon_daily[1] = 2;
  let icon2_img = new Image();
  icon2_img.onload = function(){
    context.drawImage(icon2_img, 123, 247);
  };
  if(icon_daily[1] > 0 && icon_daily[1] <= 50) icon2_img.src = `${git}i${icon_daily[1]}.png?raw=true`;
  if(icon_daily[2] == 3) icon_daily[2] = 2;
  let icon3_img = new Image();
  icon3_img.onload = function(){
    context.drawImage(icon3_img, 243, 247);
  };
  if(icon_daily[2] > 0 && icon_daily[2] <= 50) icon3_img.src = `${git}i${icon_daily[2]}.png?raw=true`;
  if(icon_daily[3] == 3) icon_daily[3] = 2;
  let icon4_img = new Image();
  icon4_img.onload = function(){
    context.drawImage(icon4_img, 363, 247);
  };
  if(icon_daily[3] > 0 && icon_daily[3] <= 50) icon4_img.src = `${git}i${icon_daily[3]}.png?raw=true`;

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
  if(temp_max_daily[0] > -50 && temp_max_daily[0] < 100){
    context.fillText(`${temp_max_daily[0]}°`, 86 - context.measureText(`${temp_max_daily[0]}°`).width / 2, 262);
  }
  if(temp_max_daily[1] > -50 && temp_max_daily[1] < 100){
    context.fillText(`${temp_max_daily[1]}°`, 206 - context.measureText(`${temp_max_daily[1]}°`).width / 2, 262);
  }
  if(temp_max_daily[2] > -50 && temp_max_daily[2] < 100){
    context.fillText(`${temp_max_daily[2]}°`, 326 - context.measureText(`${temp_max_daily[2]}°`).width / 2, 262);
  }
  if(temp_max_daily[2] > -50 && temp_max_daily[2] < 100){
    context.fillText(`${temp_max_daily[3]}°`, 446 - context.measureText(`${temp_max_daily[3]}°`).width / 2, 262);
  }
  context.fillStyle = "rgb(255, 127, 0)";
  if(temp_min_daily[0] > -50 && temp_min_daily[0] < 100){
    context.fillText(`${temp_min_daily[0]}°`, 86 - context.measureText(`${temp_min_daily[0]}°`).width / 2, 289);
  }
  if(temp_min_daily[1] > -50 && temp_min_daily[1] < 100){
    context.fillText(`${temp_min_daily[1]}°`, 206 - context.measureText(`${temp_min_daily[1]}°`).width / 2, 289);
  }
  if(temp_min_daily[2] > -50 && temp_min_daily[2] < 100){
    context.fillText(`${temp_min_daily[2]}°`, 326 - context.measureText(`${temp_min_daily[2]}°`).width / 2, 289);
  }
  if(temp_min_daily[3] > -50 && temp_min_daily[3] < 100){
    context.fillText(`${temp_min_daily[3]}°`, 446 - context.measureText(`${temp_min_daily[3]}°`).width / 2, 289);
  }
  context.font = '22px Ubuntu';
  context.fillStyle = "white";
  if(w_speed_daily[0] >= 0 && w_speed_daily[0] < 100){
    context.fillText(`${w_speed_daily[0]}м/с`, 63 - context.measureText(`${w_speed_daily[0]}м/с`).width / 2, 314);
  }
  if(w_speed_daily[1] >= 0 && w_speed_daily[1] < 100){
    context.fillText(`${w_speed_daily[1]}м/с`, 183 - context.measureText(`${w_speed_daily[1]}м/с`).width / 2, 314);
  }
  if(w_speed_daily[2] >= 0 && w_speed_daily[2] < 100){
    context.fillText(`${w_speed_daily[2]}м/с`, 303 - context.measureText(`${w_speed_daily[2]}м/с`).width / 2, 314);
  }
  if(w_speed_daily[3] >= 0 && w_speed_daily[3] < 100){
    context.fillText(`${w_speed_daily[3]}м/с`, 423 - context.measureText(`${w_speed_daily[3]}м/с`).width / 2, 314);
  }
}

$(function(){
  let config;
  let canvas = document.getElementById("display_canvas");
  let context = canvas.getContext("2d");
  context.rect(0, 0, 480, 320);
  context.fillStyle = 'black';
  context.fill();
  let temp_max_daily = [-50, -50, -50, -50],
      temp_min_daily = [100, 100, 100, 100],
      icon_daily = [0, 0, 0, 0],
      w_speed_daily = [-1, -1, -1, -1],
      descript, icon, w_speed, w_dir;

  $.getJSON(`config.json?R=${Math.random()}`,function(json){
    config = json;
    for(var b in config){
      try{$(`#${b}`).val(config[b]);}
      catch(b){}
    }
    $('#auto_day').val(config.brday);
    $('#auto_night').val(config.brnight);
    $('#time_day').val(config.brday);
    $('#time_night').val(config.brnight);
    $('#const_day').val(config.brday);
    $('#day_time').val(((config.hd < 10) ? ('0' + config.hd) : config.hd) + ':' + config.md);
    $('#night_time').val(((config.hn < 10) ? ('0' + config.hn) : config.hn) + ':' + config.mn);
    $(`input[name="brt"]`)[config.brt].checked = true;
    bright_type(config.brt);
    $(`input[name="dt0"]`)[config.dt0].checked = true;
    $(`input[name="dt1"]`)[config.dt1].checked = true;
    $(`input[name="dt2"]`)[config.dt2].checked = true;
    $(`input[name="dt3"]`)[config.dt3].checked = true;
    $(`input[name="dt4"]`)[config.dt4].checked = true;
    $(`input[name="dt5"]`)[config.dt5].checked = true;
    $(`#ds0_${config.dt0}`).val(config.ds0);
    $(`#ds1_${config.dt1}`).val(config.ds1);
    $(`#ds2_${config.dt2}`).val(config.ds2);
    $(`#ds3_${config.dt3}`).val(config.ds3);
    $(`#ds4_${config.dt4}`).val(config.ds4);
    $(`#ds5_${config.dt5}`).val(config.ds5);
    $(`#dc0_${config.dt0}`).css('background-color', rgb16_to_color(config.dc0));
    $(`#dc1_${config.dt1}`).css('background-color', rgb16_to_color(config.dc1));
    $(`#dc2_${config.dt2}`).css('background-color', rgb16_to_color(config.dc2));
    $(`#dc3_${config.dt3}`).css('background-color', rgb16_to_color(config.dc3));
    $(`#dc4_${config.dt4}`).css('background-color', rgb16_to_color(config.dc4));
    $(`#dc5_${config.dt5}`).css('background-color', rgb16_to_color(config.dc5));
    $(`#ds0_${config.dt0}`).removeAttr('disabled');
    $(`#ds1_${config.dt1}`).removeAttr('disabled');
    $(`#ds2_${config.dt2}`).removeAttr('disabled');
    $(`#ds3_${config.dt3}`).removeAttr('disabled');
    $(`#ds4_${config.dt4}`).removeAttr('disabled');
    $(`#ds5_${config.dt5}`).removeAttr('disabled');


    let ground = new Image();
    ground.onload = function(){
	    context.drawImage(ground, 0, 0);
    };
    ground.src = `${git}disp.png?raw=true`;
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

    clock_upd(context);

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
        descript = json["weather"][0]["description"];
        icon = json["weather"][0]["icon"];
        w_speed = Math.round(json["wind"]["speed"]);
        w_dir = json["wind"]["deg"];
      }
      if(config.provider == 1){
        descript = json["data"][0]["weather"]["description"];
        let pod = json["data"][0]["pod"];
        let icn = weatherbit_icon(json["data"][0]["weather"]["code"]);
        if(icn < 10) icon = '0' + icn + pod;
        else icon = icn + pod;
        w_speed = Math.round(json["data"][0]["wind_spd"]);
        w_dir = json["data"][0]["wind_dir"];
      }

      curr_weather(context, icon, w_dir, descript, w_speed);
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

      forecast(context, icon_daily, temp_max_daily, temp_min_daily, w_speed_daily);
    });

    $('#loading').removeClass('active');
  });

  $('form').submit(function(){
    $('#loading').addClass('active');
    config.brt = Number($('input[name=brt]:checked').val());
    config.brday = Number($('#auto_day').val());
    config.brnight = Number($('#auto_night').val());
    config.hd = Number($('#day_time').val().split(':')[0]);
    config.md = Number($('#day_time').val().split(':')[1]);
    config.hn = Number($('#night_time').val().split(':')[0]);
    config.mn = Number($('#night_time').val().split(':')[1]);
    config.light_in = Number($('#light_in').val());
    config.temp_out = Number($('#temp_out').val());
    config.temp_in = Number($('#temp_in').val());
    config.hum_out = Number($('#hum_out').val());
    config.hum_in = Number($('#hum_in').val());
    config.pres_out = Number($('#pres_out').val());
    config.bat_disp = Number($('#bat_disp').val());
    config.dp0 = Number($('#dp0').val());
    config.dp1 = Number($('#dp1').val());
    config.dp2 = Number($('#dp2').val());
    config.dp3 = Number($('#dp3').val());
    config.dp4 = Number($('#dp4').val());
    config.dp5 = Number($('#dp5').val());
    config.dt0 = Number($('input[name=dt0]:checked').val());
    config.dt1 = Number($('input[name=dt1]:checked').val());
    config.dt2 = Number($('input[name=dt2]:checked').val());
    config.dt3 = Number($('input[name=dt3]:checked').val());
    config.dt4 = Number($('input[name=dt4]:checked').val());
    config.dt5 = Number($('input[name=dt5]:checked').val());
    config.ds0 = $(`#ds0_${config.dt0}`).val();
    config.ds1 = $(`#ds1_${config.dt1}`).val();
    config.ds2 = $(`#ds2_${config.dt2}`).val();
    config.ds3 = $(`#ds3_${config.dt3}`).val();
    config.ds4 = $(`#ds4_${config.dt4}`).val();
    config.ds5 = $(`#ds5_${config.dt5}`).val();
    config.dc0 = color_to_rgb16($(`#dc0_${config.dt0}`).css('background-color'));
    config.dc1 = color_to_rgb16($(`#dc1_${config.dt1}`).css('background-color'));
    config.dc2 = color_to_rgb16($(`#dc2_${config.dt2}`).css('background-color'));
    config.dc3 = color_to_rgb16($(`#dc3_${config.dt3}`).css('background-color'));
    config.dc4 = color_to_rgb16($(`#dc4_${config.dt4}`).css('background-color'));
    config.dc5 = color_to_rgb16($(`#dc5_${config.dt5}`).css('background-color'));
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

  $('input[name="brt"]').click(function(){
    bright_type(this.value);
  });

  $('#auto_day').click(function(){
    $('#time_day').val($('#auto_day').val());
    $('#const_day').val($('#auto_day').val());
  });

  $('#time_day').click(function(){
    $('#auto_day').val($('#time_day').val());
    $('#const_day').val($('#time_day').val());
  });

  $('#const_day').click(function(){
    $('#auto_day').val($('#const_day').val());
    $('#time_day').val($('#const_day').val());
  });

  $('#time_night').click(function(){
    $('#auto_night').val($('#time_night').val());
  });

  $('#auto_night').click(function(){
    $('#time_night').val($('#auto_night').val());
  });

  $('input[name="dt0"]').change(function(){
    config.dt0 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt0){
        $(`#dc0_${i}`).css('background-color', rgb16_to_color(config.dc0));
        $(`#ds0_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc0_${i}`).css('background-color', '#CCC');
        $(`#ds0_${i}`).attr('disabled', true);
      }
    }
  });

  $('input[name="dt1"]').change(function(){
    config.dt1 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt1){
        $(`#dc1_${i}`).css('background-color', rgb16_to_color(config.dc1));
        $(`#ds1_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc1_${i}`).css('background-color', '#CCC');
        $(`#ds1_${i}`).attr('disabled', true);
      }
    }
  });

  $('input[name="dt2"]').change(function(){
    config.dt2 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt2){
        $(`#dc2_${i}`).css('background-color', rgb16_to_color(config.dc2));
        $(`#ds2_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc2_${i}`).css('background-color', '#CCC');
        $(`#ds2_${i}`).attr('disabled', true);
      }
    }
  });

  $('input[name="dt3"]').change(function(){
    config.dt3 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt3){
        $(`#dc3_${i}`).css('background-color', rgb16_to_color(config.dc3));
        $(`#ds3_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc3_${i}`).css('background-color', '#CCC');
        $(`#ds3_${i}`).attr('disabled', true);
      }
    }
  });

  $('input[name="dt4"]').change(function(){
    config.dt4 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt4){
        $(`#dc4_${i}`).css('background-color', rgb16_to_color(config.dc4));
        $(`#ds4_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc4_${i}`).css('background-color', '#CCC');
        $(`#ds4_${i}`).attr('disabled', true);
      }
    }
  });

  $('input[name="dt5"]').change(function(){
    config.dt5 = this.value;
    for(let i=1; i<8; i++){
      if(i == config.dt5){
        $(`#dc5_${i}`).css('background-color', rgb16_to_color(config.dc5));
        $(`#ds5_${i}`).removeAttr('disabled');
      }
      else{
        $(`#dc5_${i}`).css('background-color', '#CCC');
        $(`#ds5_${i}`).attr('disabled', true);
      }
    }
  });

  $('#auto_day, #auto_night, #time_day, #time_night, #const_day').change(function(){
    $.ajax({
	    url: 'esp/bright.php',
	    method: 'get',
	    data: `br=${this.value}`,
	    success: function(data){}
    });
  });

  $(document).click(function(e){
    if(!$('#navmenu').is(e.target) && !$('#nav-toggle').is(e.target) && !$('#mn').is(e.target)){
      document.getElementById('nav-toggle').checked = false;
    }

    if(e.target.id.includes('dc')){
      let a = $(`#${e.target.id}`).css('background-color');
      if(a == 'rgb(255, 255, 255)') $(`#${e.target.id}`).css('background-color', '#F00');
      if(a == 'rgb(255, 0, 255)') $(`#${e.target.id}`).css('background-color', '#FFF');
      if(a == 'rgb(0, 0, 255)') $(`#${e.target.id}`).css('background-color', '#F0F');
      if(a == 'rgb(0, 255, 255)') $(`#${e.target.id}`).css('background-color', '#00F');
      if(a == 'rgb(0, 255, 0)') $(`#${e.target.id}`).css('background-color', '#0FF');
      if(a == 'rgb(255, 255, 0)') $(`#${e.target.id}`).css('background-color', '#0F0');
      if(a == 'rgb(255, 0, 0)') $(`#${e.target.id}`).css('background-color', '#FF0');
    }
  });

  setInterval(function(){
    clock_upd(context);
    curr_weather(context, icon, w_dir, descript, w_speed);
    forecast(context, icon_daily, temp_max_daily, temp_min_daily, w_speed_daily);
  }, 1000);

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
});
