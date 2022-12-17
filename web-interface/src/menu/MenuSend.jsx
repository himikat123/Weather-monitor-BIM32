import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as SendImage} from "../svg/send.svg";

function MenuSend(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isSend = '';
    if(location == '/send') isSend = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/send'} className="nav-link active d-flex" title={text.get('dataSend', lang)}>
                <div className={"menu-icon" + isSend}>
                    <SendImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('dataSend', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuSend;