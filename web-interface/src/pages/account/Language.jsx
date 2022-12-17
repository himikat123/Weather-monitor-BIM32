import LanguageFn from "./LanguageFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import {ReactComponent as FlagEn} from "../../svg/en.svg";
import {ReactComponent as FlagDe} from "../../svg/de.svg";
import {ReactComponent as FlagRu} from "../../svg/ru.svg";
import {ReactComponent as FlagPl} from "../../svg/pl.svg";
import {ReactComponent as FlagUa} from "../../svg/ua.svg";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Language extends LanguageFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('language', lang)} />

                    {this.props.config.hasOwnProperty('account') && 
                    <div className="row d-flex justify-content-center">
                        <div className="col border border-secondary rounded m-4 p-4 account">
                            
                            {/* Language change */}

                            {/* English */}
                            <div className="row my-3">
                                <div className="col-2 pt-1">
                                    <input id="en" className="form-check-input" name="lang" value="en" type="radio" 
                                      checked={this.props.config.lang === "en"} onChange={this.changedLang}
                                    />
                                </div>
                                <div className="col-3">
                                    <label htmlFor="en" className="flag"><FlagEn /></label>
                                </div>
                                <div className="col-7 pt-1">
                                    <label htmlFor="en">English</label>
                                </div>
                            </div>

                            {/* Deutsch */}
                            <div className="row my-3">
                                <div className="col-2 pt-1">
                                    <input id="de" className="form-check-input" name="lang" value="de" type="radio"
                                      checked={this.props.config.lang === "de"} onChange={this.changedLang}
                                    />
                                </div>
                                <div className="col-3">
                                    <label htmlFor="de" className="flag"><FlagDe /></label>
                                </div>
                                <div className="col-7 pt-1">
                                    <label htmlFor="de">Deutsch</label>
                                </div>
                            </div>

                            {/* Русский */}
                            <div className="row my-3">
                                <div className="col-2 pt-1">
                                    <input id="ru" className="form-check-input" name="lang" value="ru" type="radio"
                                      checked={this.props.config.lang === "ru"} onChange={this.changedLang}
                                    />
                                </div>
                                <div className="col-3">
                                    <label htmlFor="ru" className="flag"><FlagRu /></label>
                                </div>
                                <div className="col-7 pt-1">
                                    <label htmlFor="ru">Русский</label>
                                </div>
                            </div>

                            {/* Polski */}
                            <div className="row my-3">
                                <div className="col-2 pt-1">
                                    <input id="pl" className="form-check-input" name="lang" value="pl" type="radio"
                                      checked={this.props.config.lang === "pl"} onChange={this.changedLang}
                                    />
                                </div>
                                <div className="col-3">
                                    <label htmlFor="pl" className="flag"><FlagPl /></label>
                                </div>
                                <div className="col-7 pt-1">
                                    <label htmlFor="pl">Polski</label>
                                </div>
                            </div>

                            {/* Українська */}
                            <div className="row my-3">
                                <div className="col-2 pt-1">
                                    <input id="ua" className="form-check-input" name="lang" value="ua" type="radio"
                                      checked={this.props.config.lang === "ua"} onChange={this.changedLang}
                                    />
                                </div>
                                <div className="col-3">
                                    <label htmlFor="ua" className="flag"><FlagUa /></label>
                                </div>
                                <div className="col-7 pt-1">
                                    <label htmlFor="ua">Українська</label>
                                </div>
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Language;