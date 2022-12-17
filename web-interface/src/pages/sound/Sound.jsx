import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";
import SoundFn from "./SoundFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import SelectInput from "../../fragments/SelectInput";
import RangeInput from "../../fragments/RangeInput";

class Sound extends SoundFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let options = [
            text.get('alwaysOn', lang),
            text.get('alwaysOff', lang),
            text.get('onFromDawnToDusk', lang),  
            text.get('enabledByTime', lang)
        ];

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('sound', lang)} />

                    {this.props.config.hasOwnProperty('sound') &&
                    <div className="row">

                        {/* Volume control */}
                        <div className="col-12 col-md-6 p-4">
                            <div className="sensor border border-secondary rounded p-3 h-100">
                                <RangeInput value={'sound.vol'}
                                    label={text.get('volume', lang)} 
                                    min="1"
                                    max="30"
                                    limitMin="1"
                                    limitMax="30"
                                    step="1"
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                    callback={this.changedVolume}
                                />
                            </div>
                        </div>

                        {/* Equalizer */}
                        <div className="col-12 col-md-6 p-4">
                            <div className="sensor border border-secondary rounded p-3 h-100 text-center">
                                <label className="form-label mt-3">
                                    {text.get('equalizer', lang)}
                                </label>
                                <div className="d-flex justify-content-center">
                                    <button className={'eq-0 btn m-2 ' + 
                                      (this.props.config.sound.eq == 0 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        NORMAL
                                    </button>
                                    <button className={'eq-1 btn m-2 ' + 
                                      (this.props.config.sound.eq == 1 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        POP
                                    </button>
                                    <button className={'eq-2 btn m-2 ' + 
                                      (this.props.config.sound.eq == 2 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        POCK
                                    </button>
                                </div>
                                <div className="d-flex justify-content-center">
                                    <button className={'eq-3 btn m-2 ' + 
                                      (this.props.config.sound.eq == 3 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        JAZZ
                                    </button>
                                    <button className={'eq-4 btn m-2 ' + 
                                      (this.props.config.sound.eq == 4 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        CLASSIC
                                    </button>
                                    <button className={'eq-5 btn m-2 ' + 
                                      (this.props.config.sound.eq == 5 ? 'btn-success' : 'btn-secondary')} onClick={this.clickEQ}>
                                        BASS
                                    </button>
                                </div>
                            </div>    
                        </div>

                        {/* Hourly signal */}
                        <div className="col-12 col-md-6 p-4">
                            <div className="sensor border border-secondary rounded p-3 h-100">
                                <SelectInput value={'sound.hourly'}
                                    label={text.get('hourlySignal', lang)}
                                    options={options} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                />
                            
                                {this.props.config.sound.hourly == 3 && 
                                <div className="text-center">
                                    <label className="form-label mx-2 mt-4">
                                        {text.get('from', lang)}
                                    </label>
                                    <input type="time" 
                                        className="mt-4 time-control" 
                                        step="3600" 
                                        value={
                                            (this.props.config.sound.hour.from < 10 ? 
                                                '0' + this.props.config.sound.hour.from : 
                                                this.props.config.sound.hour.from
                                            ) + ':00'}
                                        onChange={this.changedHrFrom} 
                                    />
                                    <label className="form-label mx-2 mt-4">
                                        {text.get('to', lang)}
                                    </label>
                                    <input type="time" 
                                        className="mt-4 time-control" 
                                        step="3600"
                                        value={
                                            (this.props.config.sound.hour.to < 10 ? 
                                                '0' + this.props.config.sound.hour.to : 
                                                this.props.config.sound.hour.to
                                            ) + ':00'}
                                        onChange={this.changedHrTo} 
                                    />
                                </div>}
                            </div>
                        </div>

                        {/* Sound Player */}
                        <div className="col-12 col-md-6 p-4">
                            <div className="sensor border border-secondary rounded p-3 h-100">
                                <label className="form-label">
                                    {text.get('listenToTheSoundOfTheClock', lang)}
                                </label>
                                <div className="d-flex mb-3">
                                    <select className="form-select form-select-sm me-1"
                                      value={this.state.hourlySound}
                                      onChange={this.changedHourlySound}>
                                        <option value="25">{text.get('hourlySignal', lang)}</option>
                                        {[...Array(24)].map((x, i) =>
                                            <option key={i} value={i}>
                                                {this.getHours(i)}
                                            </option> 
                                        )}
                                    </select>
                                    <button type="button" className="btn btn-primary me-1" onClick={this.playHourly}>
                                        <div className="play">►</div>
                                    </button>
                                    <button type="button" className="btn btn-secondary" onClick={this.stopPlaying}>■</button>
                                </div>
                            
                                <label className="form-label">{text.get('listenToAlarmMelodies', lang)}</label>
                                <div className="d-flex">
                                    <select className="form-select form-select-sm me-1"
                                      value={this.state.alarmMelody}
                                      onChange={this.changedAlarmMelody}>
                                        {[...Array(20)].map((x, i) =>
                                            <option key={i} value={i + 1}>
                                                {text.get('melody', lang) + ' ' + (i + 1)}
                                            </option> 
                                        )}
                                    </select>
                                    <button type="button" className="btn btn-primary me-1" onClick={this.playAlarm}>
                                        <div className="play">►</div>
                                    </button>
                                    <button type="button" className="btn btn-secondary" onClick={this.stopPlaying}>
                                        <div>■</div></button>
                                </div>
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    }
};

export default Sound;