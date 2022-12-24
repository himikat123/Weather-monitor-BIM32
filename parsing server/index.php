<?php
    if(isset($_GET['provider']) && isset($_GET['appid']) && isset($_GET['mac'])) {
        if(isset($_GET['q']) || isset($_GET['id']) || (isset($_GET['lat']) && isset($_GET['lon']))) {
            $location = '';
            if(isset($_GET['q'])) $location = '?q=' . $_GET['q'];
            if(isset($_GET['id'])) $location = '?id=' . $_GET['id'];
            if(isset($_GET['lat']) && isset($_GET['lon'])) $location = '?lat=' . $_GET['lat'] . '&lon=' . $GET['lon'];
            $provider = intval($_GET['provider']);
            $appid = $_GET['appid'];
            $mac = $_GET['mac'];
            $out = [];
            $num = 0;

            if(ctype_xdigit($appid) && strlen($appid) == 32) {
                if($provider == 0) {
                $json = json_decode(file_get_contents("https://api.openweathermap.org/data/2.5/forecast$location&appid=$appid&units=metric"), TRUE);
                    foreach($json['list'] as $item) {
                        $out['temp'][$num] = isset($item['main']['temp']) ? $item['main']['temp'] : 40400;
                        $out['pres'][$num] = isset($item['main']['pressure']) ? $item['main']['pressure'] : 40400;
                        $out['date'][$num] = isset($item['dt']) ? $item['dt'] : 0;
                        $out['icon'][$num] = isset($item['weather'][0]['icon']) ? intval($item['weather'][0]['icon']) : 0;
                        $out['wSpeed'][$num] = isset($item['wind']['speed']) ? $item['wind']['speed'] : -1;
                        $out['wDir'][$num] = isset($item['wind']['deg']) ? $item['wind']['deg'] : -1;
                        $out['prec'][$num] = isset($item['rain']) ? $item['rain']['3h'] : (isset($item['snow']) ? $item['snow']['3h'] : 0);
                        $num++;
                    }
                    echo json_encode($out);
                }
                else echo 'wrong provider';
            }
            else echo 'wrong location';
        }
        else echo 'wrong appid';
    }
    else echo 'no data';
?>