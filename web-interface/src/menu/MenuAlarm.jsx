import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as AlarmImage} from "../svg/alarm.svg";

function MenuAlarm(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isAlarm = '';
    if(location == '/alarm') isAlarm = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/alarm'} className="nav-link active d-flex" title={text.get('alarm', lang)}>
                <div className={"menu-icon" + isAlarm}>
                    <AlarmImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('alarm', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuAlarm;