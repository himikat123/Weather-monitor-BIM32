import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as ClockImage} from "../svg/clock.svg";

function MenuClock(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isClock = '';
    if(location == '/clock') isClock = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/clock'} className="nav-link active d-flex" title={text.get('clock', lang)}>
                <div className={"menu-icon" + isClock}>
                    <ClockImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('clock', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuClock;