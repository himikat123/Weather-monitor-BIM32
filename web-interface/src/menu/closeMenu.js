export const closeMenu = () => {
    try{
        if(!document.getElementById('mobile_menu_but').classList.contains('collapsed'))
            document.getElementById('mobile_menu_but').click();
    }
    catch(e){}
}