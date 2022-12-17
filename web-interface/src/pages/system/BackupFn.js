import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";
import axios from 'axios';

class BackupFn extends Save {
    constructor(props) {
        super(props);
        this.state = { 
            configFile: '', 
            menuOpen: 1,
            recipes: '',
            saveButtonColor: 'btn-primary'
        }

        this.changedFile = this.changedFile.bind(this);
        this.restore = this.restore.bind(this);
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
     * File has been changed
     */
    changedFile(event) {
        let files = event.target.files;
        let reader = new FileReader();
        reader.onload = (() => {
            return (e) => {
                this.setState({ configFile: e.target.result });
            };
        })(files[0]);
        reader.readAsText(files[0]);
    }

    /**
     * Restore button has been pressed
     */
    restore() {
        this.setState({ saveButtonColor: 'btn-warning' });
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios({
            method: 'post',
            mode: 'cors',
            url: `${href}/esp/saveConfig`,
            data: `config=${this.state.configFile}`
        })
        .then(
            res => {
                this.setState({ recipes: res.data });
                this.buttonColor();
            }
        );
    }

    /**
     * change restore button color
     */
    buttonColor(){
        if(this.state.recipes && this.state.recipes == 'OK') 
            this.setState({ saveButtonColor: 'btn-success' });
        else this.setState({ saveButtonColor: 'btn-danger' });
        setTimeout(() => {
            this.setState({ saveButtonColor: 'btn-primary' });
        }, 3000);
    }
};

export default BackupFn;