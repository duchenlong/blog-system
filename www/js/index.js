function tab_event() {
    var tabs = document.getElementById('tabs-blog').getElementsByTagName('input');
    var title = document.getElementById('add-title').querySelector('input');
    // console.log(both.checked);

    title.value = "";
    for (var i = 0; i < tabs.length; i++) {
        tabs[i].checked = false;
        tabs[i].onclick = function() {
            if (this.checked === true) {
                for (var j = 0; j < tabs.length; j++) {
                    tabs[j].checked = false;
                }
                this.checked = true;
            } else if (this.checked === false) {
                idx = -1;
            }
        }
    }
};

function Get_tag_idx() {
    var tabs = document.getElementById('tabs-blog').getElementsByTagName('input');
    // console.log(both.checked);

    for (var i = 0; i < tabs.length; i++) {
        if (tabs[i].checked === true) {
            return i;
        }
    }
    return -1;
}

function Get_blog_title() {

    var title = document.getElementById('add-title').querySelector('input');
    //console.log(title.value);

    return title.value;
}