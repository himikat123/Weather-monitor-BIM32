import FileSystemFn from "./FileSystemFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import ModalFileViewer from "../../modals/ModalFileViewer";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class FileSystem extends FileSystemFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        
        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('fileSystem', lang)} />

                    <div className="row justify-content-center">
                        <div className="col border border-secondary rounded m-4 p-3">

                            {/* File upload */}
                            <div className="row">
                                <div className="my-1 mt-sm-0 col-12 col-sm-6 col-xl-3">
                                    <input id="uploadFile" name="upload" className="form-control mt-3" type="file"/>
                                </div>
                                <div className="my-1 mt-sm-3 col-12 col-sm-6 col-md-3">
                                    <button className="btn btn-secondary w-100" onClick={this.upload}>
                                        {text.get('upload', lang)} {this.state.percentage}
                                    </button>
                                </div>
                            </div><hr />

                            {/* Control buttons */}
                            <div className="row">
                                {/* Open button */}
                                <div className="my-1 col-12 col-sm-6 col-md-3">
                                    <button className="btn btn-secondary w-100" onClick={this.openBtn}>
                                        {text.get('open', lang)}
                                    </button>
                                </div>
                                {/* Download button */}
                                <div className="my-1 col-12 col-sm-6 col-md-3">
                                    <button className="btn btn-secondary w-100" onClick={this.downloadBtn}>
                                        {text.get('download', lang)}
                                    </button>
                                </div>
                                {/* Rename button */}
                                <div className="my-1 col-12 col-sm-6 col-md-3">
                                    <button className="btn btn-secondary w-100" onClick={this.renameBtn}>
                                        {text.get('rename', lang)}
                                    </button>
                                </div>
                                {/* Delete button */}
                                <div className="my-1 col-12 col-sm-6 col-md-3">
                                    <button className="btn btn-secondary w-100" onClick={this.deleteBtn}>
                                        {text.get('delete', lang)}
                                    </button>
                                </div>
                            </div><hr />

                            {/* List of files */}
                            {this.props.data.hasOwnProperty('fs') &&
                                <div className="mt-3">
                                    <div className="text-end me-3">
{this.props.data.fs.free}{text.get('kb', lang)} {text.get('freeOf', lang)} {this.props.data.fs.total}{text.get('kb', lang)}
                                    </div><hr />
                                    <div className="row mb-2">
                                        <div className="col-8 text-center">{text.get('filename', lang)}</div>
                                        <div className="col-4 text-center">{text.get('size', lang)}</div>
                                    </div>
                                    {this.props.data.fs.list.map((x, i) =>
                                        <div key={i} 
                                         data-index={i}
                                         className={"row mb-1 hoverable" + (this.state.fileSelected == i ? " active" : "")}
                                         onClick={this.selectFile}
                                        >
                                            <div className="col-8 text-center text-info">
                                                {/* File download */}
                                                <a id={x.name} href={"/" + x.name} download></a>
                                                {/* Filename */}
                                                {this.state.renameNum != i && x.name}
                                                {/* File rename */}
                                                {this.state.renameNum == i && <>
                                                    <div className="input-group">
                                                    <input type="text"
                                                        data-index={i} 
                                                        className="form-control rename-field"
                                                        maxLength="32"
                                                        defaultValue={x.name}
                                                        onChange={this.changedName}
                                                        autoFocus
                                                    />
                                                    <button className="btn btn-success rename-btn" 
                                                        onClick={this.renameOk}
                                                        title={text.get('save', lang)}
                                                    >✓</button>
                                                    <button className="btn btn-danger rename-btn" 
                                                        onClick={this.renameCancel}
                                                        title={text.get('cancel', lang)}
                                                    >×</button>
                                                    </div>
                                                </>}
                                            </div>
                                            <div className="col-4 text-center text-info">
                                                {x.size} {text.get('kb', lang)}
                                            </div>
                                        </div>
                                    )}

                                    {/* Modal file viewer */}
                                    <button id="fileViewerOpen" 
                                        style={{ display: 'none'}} 
                                        data-bs-toggle="modal" 
                                        data-bs-target={"#fileviewer"} 
                                    />
                                    <ModalFileViewer lang={lang} 
                                        fileName={this.props.data.fs.list[this.state.fileSelected].name} 
                                        data={this.state.fileData} 
                                    />
                                </div>
                            }
                        </div>
                    </div>
                </div>

            </div>

            <SaveRestart config={this.props.config} restart="1" />
        </>);
    }
};

export default FileSystem;