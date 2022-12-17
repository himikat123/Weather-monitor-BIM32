import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as StatusImage} from "../svg/info.svg";

function MenuStatus(props) {
    let text = new Languages();
    let lang = props.language;
    let location = useLocation().pathname;
    let isStatus = '';
    if(location == '/') isStatus = ' active-menu-item';

    return (
        <li className="nav-item mt-3 mt-lg-0">
            <Link to={'/'} className="nav-link active d-flex" title={text.get('status', lang)}>
                <div className={"menu-icon" + isStatus}>
                    <StatusImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('status', lang)}</div>
            </Link>
        </li>
    );
};

export default MenuStatus;