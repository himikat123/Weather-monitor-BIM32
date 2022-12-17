import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as FanImage} from "../svg/fan.svg";

function MenuComfort(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isComfort = '';
    if(location == '/comfort') isComfort = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/comfort'} className="nav-link active d-flex" title={text.get('comfort', lang)}>
                <div className={"menu-icon" + isComfort}>
                    <FanImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('comfort', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuComfort;