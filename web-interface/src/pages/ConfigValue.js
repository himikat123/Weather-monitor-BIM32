import React from "react";

class ConfigValue extends React.Component {
    setVal(config, propName, event) {
        let value = propName.split('.');
        switch(value.length) {
            case 1: config[value] = event; break;
            case 2: config[value[0]][value[1]] = event; break;
            case 3: config[value[0]][value[1]][value[2]] = event; break;
            case 4: config[value[0]][value[1]][value[2]][value[3]] = event; break;
            case 5: config[value[0]][value[1]][value[2]][value[3]][value[4]] = event; break;
            case 6: config[value[0]][value[1]][value[2]][value[3]][value[4]][value[5]] = event; break;
            default: ; break;
        }
        return(config);
    }

    getVal(config, propName) {
        let value = propName.split('.');
        let level = '', val = 0;
        try {
            for(let i=0; i<value.length; i++) {
                if(!isNaN(value[i])) level += `[${value[i]}]`;
                else level += `.${value[i]}`;
            }
            val = eval(`config${level}`);
        }
        catch(e) {}
        return val;
    }
};

export default ConfigValue;