import Languages from "../Languages";
import AlarmBlockFn from "./AlarmBlockFn";

class AlarmBlock extends AlarmBlockFn {
    render() {
        let text = new Languages();
        let lang = this.props.lang;
        const weekdays = ['mo', 'tu', 'we', 'th', 'fr', 'sa', 'su'];

        return (
            <div className="col col-12 col-sm-6 col-lg-4 col-xl-3 p-3">
                <div className="border border-secondary rounded p-3">
                    
                    {/* Loading spinner */}
                    {this.state.loading && 
                        <div className="d-flex justify-content-center align-items-center" style={{height: '170px'}}>
                            <div className="spinner-border"></div>
                        </div>
                    }

                    {/* Time, On/Off switch, weekdays */}
                    {!this.state.loading && <><div className="row">

                        {/* Time */}
                        <div className="col-auto me-auto">
                            <input type="time" 
                                className="form-control"
                                value={
                                    (('0' + this.state.hr).slice(-2) + ':' + ('0' + this.state.mn).slice(-2))
                                } 
                                onChange={this.changedTime} 
                            />
                        </div>

                        {/* On/Off switch */}
                        <div className="col-auto">
                            <div className="form-check form-switch">
                                <input className="form-check-input check-lg" 
                                    type="checkbox" 
                                    checked={this.state.state}
                                    onChange={this.changedOnOff}
                                />
                            </div>
                        </div>
                    </div>

                    {/* Weekday checkboxes */}
                    <div className="text-center mt-4 fs-6">
                        <table className="table">
                            <thead><tr style={{ fontFamily: "monospace" }}>
                                { weekdays.map((name) => this.weekdayName(name)) }
                            </tr></thead>
                            <tbody><tr>
                                { weekdays.map((name, num) => this.weekdayCheckbox(name, num)) }
                            </tr></tbody>
                        </table>
                    </div>
                    
                    {/* Melody change */}
                    <div className="d-flex">
                        <select className="form-select form-select-sm me-1"
                            value={this.state.melody}
                            onChange={this.changedMelody}>
                            {[...Array(20)].map((x, i) =>
                                <option key={i} value={i + 1}>
                                    {text.get('melody', lang) + ' ' + (i + 1)}
                                </option> 
                            )}
                        </select>
                        <button type="button" className="btn btn-primary me-1" onClick={this.play}>
                            <div className="play">►</div>
                        </button>
                        <button type="button" className="btn btn-secondary" onClick={this.stop}>■</button>
                    </div></>}
                </div>
            </div>  
        );
    }
};

export default AlarmBlock;