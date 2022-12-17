import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as DisplayImage} from "../svg/display.svg";

function MenuDisplay(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isDisplays = '', isDisplay1 = '', isDisplay2 = '';
    if(location == '/display1'){
        isDisplay1 = ' active';
        isDisplays = ' active-menu-item';
    }
    if(location == '/display2'){
        isDisplay2 = ' active';
        isDisplays = ' active-menu-item';
    }

    return (
        <li className="nav-item dropdown">
            <a className="nav-link d-flex dropdown-toggle" href="#" data-bs-toggle="dropdown" title={text.get('displays', lang)}>
                <div className={"menu-icon" + isDisplays}>
                    <DisplayImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('displays', lang)}</div>
            </a>
            <ul className="dropdown-menu">
                <li>
                    <Link to={'/display1'} className={"dropdown-item" + isDisplay1}>
                        {text.get('display', lang)} 1
                    </Link>
                </li>
                <li>
                    <Link to={'/display2'} className={"dropdown-item" + isDisplay2}>
                        {text.get('display', lang)} 2
                    </Link>
                </li>
            </ul>
        </li>
    );
};

export default MenuDisplay;