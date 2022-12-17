import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as SourceImage} from "../svg/source.svg";

function MenuSource(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isSensors = '', isWsensors = '', isWeather = '', isSource = '';
    if(location == '/sensors'){
        isSensors = ' active';
        isSource = ' active-menu-item';
    }
    if(location == '/wsensors'){
        isWsensors = ' active';
        isSource = ' active-menu-item';
    }
    if(location == '/weather'){
        isWeather = ' active';
        isSource = ' active-menu-item';
    }

    return (
        <li className="nav-item dropdown">
            <a className="nav-link d-flex dropdown-toggle" href="#" data-bs-toggle="dropdown" title={text.get('dataSources', lang)}>
                <div className={"menu-icon" + isSource}>
                    <SourceImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">
                    {text.get('dataSources', lang)}
                </div>
            </a>
            <ul className="dropdown-menu" aria-labelledby="sourceDropdown">
                <li>
                    <Link to={'/sensors'} className={"dropdown-item" + isSensors}>
                        {text.get('sensors', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/wsensors'} className={"dropdown-item" + isWsensors}>
                        {text.get('wirelessSensors', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/weather'} className={"dropdown-item" + isWeather}>
                        {text.get('weatherForecast', lang)}
                    </Link>
                </li>
            </ul>
        </li>
    );
};

export default MenuSource;