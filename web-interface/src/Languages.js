import translates from "./translates.json"

class Languages {
    get(text, language) {
        return translates[text][language];
    }
};

export default Languages;