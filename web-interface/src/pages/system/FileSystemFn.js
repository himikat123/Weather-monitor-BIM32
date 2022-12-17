import Save from "../Save";
import Languages from "../../Languages";
import { closeMenu } from "../../menu/closeMenu";
import axios from 'axios';

class FileSystemFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            fileSelected: 0,
            fileData: '',
            renameNum: -1,
            renameName: '',
            percentage: '',
            menuOpen: 1
        }

        this.upload = this.upload.bind(this);
        this.selectFile = this.selectFile.bind(this);
        this.openBtn = this.openBtn.bind(this);
        this.downloadBtn = this.downloadBtn.bind(this);
        this.renameBtn = this.renameBtn.bind(this);
        this.deleteBtn = this.deleteBtn.bind(this);
        this.changedName = this.changedName.bind(this);
        this.renameCancel = this.renameCancel.bind(this);
        this.renameOk = this.renameOk.bind(this);
    }

    /**
    * Props have been updated 
    */
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    /**
     * Upload button
     */
    async upload() {
        let formData = new FormData();
        formData.append("file", document.querySelector('#uploadFile').files[0]);
    
        const onUploadProgress = (event) => {
            const percentage = Math.round((100 * event.loaded) / event.total);
            this.setState({ percentage: percentage + '%'});
        };

        try {
            let origin = window.location.origin;
            let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
            await axios.post(`${href}/esp/fileUpload`, formData, {
                headers: {
                    "Content-Type": "multipart/form-data",
                }, 
                onUploadProgress
            });
        } catch (error) { }
    }

    /**
     * Select a file in the list
     */
    selectFile(event) {
        this.setState({ fileSelected: event.currentTarget.dataset.index });
    }

    /**
     * File open button
     */
    openBtn() {
        let filename = this.props.data.fs.list[this.state.fileSelected].name;
        fetch(`/${filename}`)
        .then(res => res.text())
        .then(
            result => {
                this.setState({ fileData: result });
                document.getElementById('fileViewerOpen').click();
            }
        )
    }

    /**
     * File download button
     */
    downloadBtn() {
        document.getElementById(this.props.data.fs.list[this.state.fileSelected].name).click();
    }

    /**
     * File rename button
     */
    renameBtn() {
        this.setState({ 
            renameNum: this.state.fileSelected,
            renameName: this.props.data.fs.list[this.state.fileSelected].name
        });
    }

    /**
     * File was renamed
     */
    changedName(event) {
        this.setState({ renameName: event.target.value });
    }

    /**
     * Rename has been canceled
     */
    renameCancel() {
        this.setState({ renameNum: -1 });
    }

    /**
     * Rename has been confirmed
     */
    renameOk() {
        let text = new Languages();
        let lang = this.props.lang;
        this.setState({ renameNum: -1 });
        for(let file in this.props.data.fs.list) {
            if(this.props.data.fs.list[file].name == this.state.renameName) { 
                if(file == this.state.fileSelected) return;
                else {
                    alert(text.get('FileAlreadyExists', lang).replace(new RegExp("XXX", "g"), this.state.renameName));
                    return;
                }
            }
        }
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios({
            method: 'post',
            url: `${href}/esp/rename`,
            data: `old=${this.props.data.fs.list[this.state.fileSelected].name}&new=${this.state.renameName}`
        })
    }

    /**
     * File delete button
     */
    deleteBtn() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let file = this.props.data.fs.list[this.state.fileSelected].name;
        if(window.confirm(text.get('confirmDeletionOfTheFile', lang).replace(new RegExp("XXX", "g"), file))) {
            let origin = window.location.origin;
            let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
            axios({
                method: 'post',
                url: `${href}/esp/delete`,
                data: `file=${file}`
            })
        }
    }
};

export default FileSystemFn;