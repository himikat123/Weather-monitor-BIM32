import FirmwareFn from "./FirmwareFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Firmware extends FirmwareFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        
        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('firmware', lang)} />

                    <div className="row justify-content-center">
                        
                        {/* Firmware file */}
                        <div className="col border border-secondary rounded m-4 p-3 backup-card firmware">
                            <form method="POST" action="#" encType="multipart/form-data" id="upload_form">
                                <div>{text.get('selectFirmwareFile', lang)}</div>
                                <input name="update"
                                    className="form-control mt-3" 
                                    type="file" 
                                    accept=".bin"
                                />
                                <div className="text-center">
                                    <button className={"btn mt-3 btn-primary"} onClick={this.firmwareUpdate}>
                                        {text.get('upgrade', lang)}
                                    </button>

                                    {/* Progress bar */}
                                    <div className="d-flex justify-content-center">
                                        <div className="d-flex flex-column">
                                            <div className={"progress-circle p" + 
                                             this.state.percentage + 
                                             (this.state.percentage >= 50 ? " over50" : "")}>
                                                <span>{this.state.percentage}%</span>
                                                <div className="left-half-clipper">
                                                    <div className="first50-bar"></div>
                                                    <div className="value-bar"></div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} restart="1" />
        </>);
    }
};

export default Firmware;