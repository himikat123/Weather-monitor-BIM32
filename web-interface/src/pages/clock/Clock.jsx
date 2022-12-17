import ClockFn from "./ClockFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import TextInput from "../../fragments/TextInput";
import SelectInput from "../../fragments/SelectInput";
import NumberInput from "../../fragments/NumberInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Clock extends ClockFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let summerTime = [text.get('disabled', lang), text.get('automatically', lang)];
        let clockFormat = [text.get('format12', lang), text.get('format24', lang)];

        return(<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('clock', lang)} />

                    {this.props.config.hasOwnProperty('clock') && 
                    <div className="row">
                        <div className="col-12 col-md-6 col-xl-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">
                                
                                {/* Time format */}
                                <SelectInput value="clock.format"
                                    label={text.get('clockFormat', lang)} 
                                    options={clockFormat} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />

                                {/* Timezone */}
                                <NumberInput min="-12" 
                                    max="13" 
                                    step="1" 
                                    placeholder={text.get('timezone', lang)}
                                    label={text.get('timezone', lang)}
                                    value="clock.utc" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />

                                {/* Daylight saving time */}
                                <SelectInput value="clock.dlst"
                                    label={text.get('daylightSavingTime', lang)}
                                    options={summerTime} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                            </div>
                        </div>
                        <div className="col-12 col-md-6 col-xl-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">

                                {/* NTP server address */}
                                <TextInput maxLength="64" 
                                    placeholder={text.get('ntpServerAddress', lang)}
                                    label={text.get('ntpServerAddress', lang)} 
                                    value="clock.ntp" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />

                                {/* NTP update period */}
                                <NumberInput min="0" 
                                    max="90000" 
                                    step="1" 
                                    placeholder={text.get('ntpUpdatePeriod', lang)}
                                    label={text.get('ntpUpdatePeriod', lang)}
                                    value="clock.ntp_period" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                <div className="mt-2">(0 - {text.get('neverSync', lang)})</div>
                            </div>
                        </div>

                        {/* Time syncronization */}
                        <div className="col-12 col-xl-4 p-4">
                            <div className="border border-secondary rounded p-3 h-100 sensor">
                                <div>⇒ <span className="indication">{this.state.sendRequest}</span></div>
                                <div>⇐ <span className="indication">{this.state.receiveRequest}</span></div>
                                <hr />

                                {/* Clock sync with NTP button */}
                                <div className="d-flex justify-content-center mt-3 px-4">
                                    <button className="btn btn-success mt-3 px-4" onClick={this.clockSyncNTP}>
                                        {text.get('syncNtp', lang)}
                                        <span className={this.state.spinnerNTP}></span>
                                    </button>
                                </div>

                                {/* Clock sync with PC button */}
                                <div className="d-flex justify-content-center mt-3 px-4">
                                    <button className="btn btn-success mt-3 px-4" onClick={this.clockSyncPC}>
                                        {text.get('syncPc', lang)}
                                        <span className={this.state.spinnerPC}></span>
                                    </button>
                                </div>
                                <hr />
                            
                                {/* Manually time set */}
                                <label className="form-label mt-3">
                                    {text.get('setTimeManually', lang)}
                                </label>
                                <input type="datetime-local" 
                                    step="1" 
                                    className="form-control" 
                                    onChange={this.timeManuallyChanged} 
                                />
                                <div className="d-flex justify-content-center mt-3 px-4">
                                    <button className="btn btn-success mt-3 px-4" onClick={this.clockSyncManually}>
                                        {text.get('set', lang)}
                                        <span className={this.state.spinnerManually}></span>
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>)
    };
};

export default Clock;