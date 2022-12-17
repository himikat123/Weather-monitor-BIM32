import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as HistoryImage} from "../svg/history.svg";

function MenuClock(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isHistory = '';
    if(location == '/history') isHistory = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/history'} className="nav-link active d-flex" title={text.get('weatherHistory', lang)}>
                <div className={"menu-icon" + isHistory}>
                    <HistoryImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('weatherHistory', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuClock;