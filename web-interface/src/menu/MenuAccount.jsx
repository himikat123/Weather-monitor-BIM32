import React from "react";
import { Link, useLocation } from 'react-router-dom';
import Languages from "../Languages";
import {ReactComponent as AccountImage} from "../svg/account.svg";
import Cookies from 'universal-cookie';


function logout() {
    const cookies = new Cookies();
    cookies.set('code', '0');
}

function MenuAccount(props) {
    let text = new Languages();
    let lang = props.language;
    let location = useLocation().pathname;
    let isAccount = '';
    let isAccountName = '';
    let isAccountPass = '';
    let isAccountLang = '';
    if(location == '/username'){
        isAccount = ' active-menu-item';
        isAccountName = ' active';
    }
    if(location == '/userpass'){
        isAccount = ' active-menu-item';
        isAccountPass = ' active';
    }
    if(location == '/language'){
        isAccount = ' active-menu-item';
        isAccountLang = ' active';
    }

    return (
        <div className="nav-item dropdown mb-3 mb-lg-0">
            <a className="nav-link d-flex dropdown-toggle p-0 pe-lg-2" href="#" data-bs-toggle="dropdown" title={text.get('account', lang)}>
                <div className={"menu-icon" + isAccount}>
                    <AccountImage />
                </div>
                <div className="d-lg-none ms-3 mt-1 text-light">{text.get('account', lang)}</div>
            </a>
            <ul className="dropdown-menu" id="account-submenu">
                <li><Link to={'/username'} className={"dropdown-item" + isAccountName}>{text.get('username', lang)}</Link></li>
                <li><Link to={'/userpass'} className={"dropdown-item" + isAccountPass}>{text.get('password', lang)}</Link></li>
                <li><Link to={'/lang'} className={"dropdown-item" + isAccountLang}>{text.get('language', lang)}</Link></li>
                <li><hr className="dropdown-divider" /></li>
                <li><Link to={'/login'} className="dropdown-item" onClick={logout}>{text.get('logout', lang)}</Link></li>
            </ul>
        </div>        
    );
};

export default MenuAccount;