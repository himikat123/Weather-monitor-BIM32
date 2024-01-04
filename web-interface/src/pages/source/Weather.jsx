import WeatherFn from "./WeatherFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import TextInput from "../../fragments/TextInput";
import SelectInput from "../../fragments/SelectInput";
import NumberInput from "../../fragments/NumberInput";
import WeatherRow from "../../fragments/WeatherRow";
import {ReactComponent as InfoImage} from "../../svg/info.svg";
import {ReactComponent as ArrowImage} from "../../svg/arrow.svg";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Weather extends WeatherFn{
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let citySearch = [
            text.get('byCityName', lang), 
            text.get('byCityId', lang), 
            text.get('byCoordinates', lang)
        ];
        
        return(<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('weatherForecast', lang)} />

                    {this.props.config.hasOwnProperty('weather') && 
                    <div className="row">
                        <div className="col-12 col-md-6 col-lg-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">

                                {/* weather forecast provider */}
                                <SelectInput value="weather.provider"
                                    label={text.get('weatherForecastSource', lang)} 
                                    options={this.state.provider} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                <label className="form-label mt-3">
                                    API KEY <a href={this.state.provider[this.props.config.weather.provider]} 
                                    target="_blank" 
                                    rel="noreferrer">
                                        {this.state.provider[this.props.config.weather.provider]}
                                    </a>
                                </label>

                                {/* openweathermap API key */}
                                {this.props.config.weather.provider == 0 &&
                                    <TextInput maxLength="32" 
                                        placeholder="APPID" 
                                        value="weather.appid.0"
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                }

                                {/* weatherbit API key */}
                                {this.props.config.weather.provider == 1 &&
                                    <TextInput maxLength="32" 
                                        placeholder="KEY" 
                                        value="weather.appid.1" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                }
                            </div>
                        </div>

                        <div className="col-12 col-md-6 col-lg-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">
                            {/* city identification */}
                            <SelectInput value="weather.citysearch" 
                                label={text.get('cityIdentification', lang)}
                                options={citySearch} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig} 
                            />

                            {/* city name */}
                            {this.props.config.weather.citysearch == 0 && <>
                                <TextInput maxLength="40" 
                                    label={text.get('cityName', lang)}
                                    placeholder={text.get('city', lang)} 
                                    value="weather.city" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                            </>}

                            {/* city ID number */}
                            {this.props.config.weather.citysearch == 1 && <>
                                <NumberInput min="0" 
                                    max="9999999999" 
                                    step="1" 
                                    placeholder={text.get('cityIdNumber', lang)}
                                    label={text.get('cityIdNumber', lang)}
                                    value="weather.cityid" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                <div className="d-flex justify-content-center mt-3">
                                    <a className="btn btn-success mt-3" 
                                     href="https://github.com/himikat123/Weather-monitor-BIM32/blob/master/CityID_RU.zip?raw=true" 
                                     target="_blank"
                                     rel="noreferrer"
                                    >
                                        {text.get('cityIdSearchProgram', lang)}
                                    </a>
                                </div>
                            </>}

                            {this.props.config.weather.citysearch == 2 && <>
                                
                                {/* latitude */}
                                <NumberInput min="-90" 
                                    max="90" 
                                    step="0.00001" 
                                    placeholder={text.get('latitude', lang)}
                                    label={text.get('latitude', lang)}
                                    value="weather.lat" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />

                                {/* longitude */}
                                <NumberInput min="-180" 
                                    max="180" 
                                    step="0.00001" 
                                    placeholder={text.get('longitude', lang)}
                                    label={text.get('longitude', lang)}
                                    value="weather.lon" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                <div className="d-flex justify-content-center mt-3">
                                    <button className="btn btn-success mt-3 px-4" onClick={this.getGeolocation}>
                                        {text.get('detectCoordinatesAutomatically', lang)}
                                        <span className={this.state.getCoordinates}></span>
                                    </button>
                                </div>
                            </>}</div>
                        </div>

                        {/* weather settings check */}
                        <div className="col-12 col-lg-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">
                            <p className={"d-flex justify-content-center " + this.state.color}>
                                {this.state.description}
                            </p>
                            <WeatherRow text={text.get('temperature', lang)} 
                                color={this.state.color} 
                                value={this.state.temperature + ' °C'} 
                            />
                            <WeatherRow text={text.get('humidity', lang)} 
                                color={this.state.color} 
                                value={this.state.humidity + ' %'} 
                            />
                            <WeatherRow text={text.get('pressure', lang)} 
                                color={this.state.color} 
                                value={this.state.pressure + ' ' + text.get('mm', lang)} 
                            />
                            <div className="row">
                                <div className="col-6 text-end">{text.get('wind', lang)}:</div>
                                <div className={"col-6 " + this.state.color + " d-flex"}>
                                    {this.state.wind + ' ' + text.get('mps', lang)}
                                    <div className="arrow-pic ms-2">
                                        <ArrowImage style={{transform: `rotate(${90 + this.state.dir}deg)` }} />
                                    </div>
                                </div>
                            </div>
                            <WeatherRow text={text.get('city', lang)} 
                                color={this.state.color} 
                                value={this.state.city + ', ' + this.state.country} 
                            />
                            <WeatherRow text={text.get('latitude', lang)} 
                                color={this.state.color} 
                                value={this.state.latitude} 
                            />
                            <WeatherRow text={text.get('longitude', lang)} 
                                color={this.state.color} 
                                value={this.state.longitude} 
                            /><hr />
                            <div className="d-flex justify-content-center mt-3">
                                <button className="btn btn-success mt-3 px-4" onClick={this.getWeather}>
                                    {text.get('check', lang)}
                                    <span className={this.state.checkWeather}></span>
                                </button>
                                <div className="menu-icon ms-2" title={text.get('checkingWeatherSettings', lang)}>
                                    <InfoImage width="20px" />
                                </div>
                            </div>
                        </div></div>
                    </div> }
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Weather;