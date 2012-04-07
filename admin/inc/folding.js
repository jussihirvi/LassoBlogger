   // admin menu - show and hide submenus
   // code copied from enqa.eu/gadocs/includes/folding.js
    $(function() {
        // Find list items representing folders and turn them
        // into links that can expand/collapse the tree leaf.
        $('li.menucat').each(function(i) {
            // Temporarily decouple the child list, wrap the
            // remaining text in an anchor, then reattach it.
            var sub_ul = $(this).children().remove();
            $(this).wrapInner('<a/>').find('a').click(function() {
                // Make the anchor toggle the leaf display.
                sub_ul.toggle();
            });
            $(this).append(sub_ul);
        });

        // Hide all lists except the outermost.
        $('ul ul').hide();
    });

