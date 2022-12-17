import BackupFn from "./BackupFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Backup extends BackupFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        
        return (<>
            <Menu language={lang} />

            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('backup', lang)} />

                    <div className="row justify-content-center">
                        
                        {/* Backup config file */}
                        <div className="col border border-secondary rounded m-4 p-3 backup-card">
                            <div>
                                {text.get('downloadSettingsBackupFile', lang)}
                            </div>
                            <div className="form-control mt-3">config.json</div>
                            <div className="text-center">
                                <a className="btn btn-success mt-3" href="config.json" download>
                                    {text.get('download', lang)}
                                </a>
                            </div>
                        </div>

                        {/* Restore config file */}
                        <div className="col border border-secondary rounded m-4 p-3 backup-card">
                            <div>
                                {text.get('restoreSettingsFromBackupFile', lang)}
                            </div>
                            <input className="form-control mt-3" 
                                type="file" 
                                id="formFile"
                                accept=".json" 
                                onChange={this.changedFile} 
                            />
                            <div className="text-center">
                                <button className={"btn mt-3 " + this.state.saveButtonColor} 
                                    onClick={this.restore}
                                >
                                    {text.get('restore', lang)}
                                </button>
                            </div>
                        </div>
                    </div>

                    {/* Config file content */}
                    {this.state.configFile != '' && <div className="row justify-content-center">
                        <div className="col border border-secondary rounded m-4 p-3 backup-card">
                            <div className="accordion" id="jsonFile">
                                <div className="accordion-item">
                                    <h2 className="accordion-header" id="headingOne">
                                        <button className="accordion-button" 
                                            type="button" 
                                            data-bs-toggle="collapse" 
                                            data-bs-target="#configCode"
                                        >
                                            {text.get('fileContents', lang)}
                                        </button>
                                    </h2>
                                    <div id="configCode" 
                                        className="accordion-collapse collapse configCode"
                                        data-bs-parent="#jsonFile"
                                    >
                                        <div className="accordion-body">
                                            <pre>{this.state.configFile}</pre>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} restart="1" />
        </>);
    }
};

export default Backup;