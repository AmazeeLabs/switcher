(function ($) {
  Drupal.behaviors.switcher = {
    attach: function (context, settings) {
      $('div.switcher-wrapper', context).once('switcher-wrapper', function(){
        var $dropdownList = $(this).find('ul.switcher');
        $dropdownList.hide();
        $(this).find('.switcher-active a').bind('touchstart', function(e){
          e.preventDefault();
          $dropdownList.fadeToggle(50);
          $dropdown.toggleClass('menu-active');
        });

        $(this).hover(function() {
          $dropdownList.fadeToggle(100);
          $(this).toggleClass('menu-active');
        });
      });
    }
  };

})(jQuery);
