import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as SoundImage} from "../svg/sound.svg";

function MenuSound(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isSound = '';
    if(location == '/sound') isSound = ' active-menu-item';
    
    return (
        <li className="nav-item">
            <Link to={'/sound'} className="nav-link active d-flex" title={text.get('sound', lang)}>
                <div className={"menu-icon" + isSound}>
                    <SoundImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('sound', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuSound;