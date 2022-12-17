import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as SystemImage} from "../svg/system.svg";

function MenuSystem(props) {
    let text = new Languages(), lang = props.language;
    let location = useLocation().pathname;
    let isSystem = '', isBackup = '', isDefault = '', isFirmware = '', isFilesystem = '';
    if(location == '/backup'){
        isBackup = ' active';
        isSystem = ' active-menu-item';
    }
    if(location == '/default'){
        isDefault = ' active';
        isSystem = ' active-menu-item';
    }
    if(location == '/firmware'){
        isFirmware = ' active';
        isSystem = ' active-menu-item';
    }
    if(location == '/filesystem'){
        isFilesystem = ' active';
        isSystem = ' active-menu-item';
    }

    return (
        <li className="nav-item dropdown">
            <a className="nav-link d-flex dropdown-toggle" href="() => return false" data-bs-toggle="dropdown" title={text.get('system', lang)}>
                <div className={"menu-icon" + isSystem}>
                    <SystemImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('system', lang)}</div>
            </a>
            <ul className="dropdown-menu" aria-labelledby="systemDropdown">
                <li>
                    <Link to={'/backup'} className={"dropdown-item" + isBackup}>
                        {text.get('backup', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/default'} className={"dropdown-item" + isDefault}>
                        {text.get('defaultSettings', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/firmware'} className={"dropdown-item" + isFirmware}>
                        {text.get('firmware', lang)}
                    </Link>
                </li>
                <li>
                    <Link to={'/filesystem'} className={"dropdown-item" + isFilesystem}>
                        {text.get('fileSystem', lang)}
                    </Link>
                </li>
            </ul>
        </li>
    );
};
export default MenuSystem;