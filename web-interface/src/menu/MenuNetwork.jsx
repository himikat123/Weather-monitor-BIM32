import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as NetworkImage} from "../svg/network.svg";

function MenuNetwork(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isConnect = '', isAccessPoint = '', isNetwork = '';
    if(location == '/connect'){
        isConnect = ' active';
        isNetwork = ' active-menu-item';
    }
    if(location == '/accesspoint'){
        isAccessPoint = ' active';
        isNetwork = ' active-menu-item';
    }
        
    return (
        <li className="nav-item dropdown">
            <a className="nav-link d-flex dropdown-toggle" href="#" data-bs-toggle="dropdown" title={text.get('network', lang)}>
                <div className={"menu-icon" + isNetwork}>
                    <NetworkImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">
                    {text.get('network', lang)}
                </div>
            </a>
            <ul className="dropdown-menu" aria-labelledby="networkDropdown">
                <li>
                    <Link to={'/connect'} className={"dropdown-item" + isConnect}>
                        {text.get('connections', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/accesspoint'} className={"dropdown-item" + isAccessPoint}>
                        {text.get('accessPoint', lang)}
                    </Link>
                </li>
            </ul>
        </li>
    );
};

export default MenuNetwork;