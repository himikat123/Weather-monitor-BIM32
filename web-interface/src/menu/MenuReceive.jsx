import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as ReceiveImage} from "../svg/receive.svg";

function MenuReceive(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isReceive = '';
    if(location == '/receive') isReceive = ' active-menu-item';

    return (
        <li className="nav-item">
            <Link to={'/receive'} className="nav-link active d-flex" title={text.get('dataReceive', lang)}>
                <div className={"menu-icon" + isReceive}>
                    <ReceiveImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('dataReceive', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuReceive;