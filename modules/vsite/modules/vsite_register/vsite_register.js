/**
  * @file vsite_register.js
  *
  * Provides events to change display of select current user/create new user in vsite registration form.
  * 
  * Drupal.behaviors.vsite_register.toggle_user_forms swaps display of the new/existing user form elements.  This is determined
  * by new user's display state.  So when both elements are showing, toggling will hide one and show the other.
  * 
  * Attaches a click event to call toggle_user_forms.  Tries to do this only once by inspecting $elem.data.  Ajax calls
  * will trigger attach again, causing multiple instances of the same click event to be registered if this isn't done.
  */

(function($) {
  Drupal.behaviors.vsite_register = {
    toggle_user_forms: function() {
    	var new_user = $('#new_user_div');
    	var old_user = $('#edit-existing-username').parent();
    	
    	if (new_user.css('display') == 'none') {
    		new_user.css('display', 'block');
    		old_user.css('display', 'none');
    	} else {
    		new_user.css('display', 'none');
    		old_user.css('display', 'block');
    	}
    	
    },
  
		attach: function(ctx, settings) {
			if (typeof Drupal.settings.vsite_register == 'undefined') {
				Drupal.settings.vsite_register = {new_user_clicked: false};
			}
			
			if ($('#new_user_div').css('display') == $('#edit-existing-username').parent().css('display')) {
				Drupal.behaviors.vsite_register.toggle_user_forms();
			}
			
			var link = $('#new-user-link');
			var link_data = link.data();
			if ((typeof link_data['events'] == 'undefined') || link_data['events'].click.length < 1) {
				$('#new-user-link').click( function() {
					$('input[name=create_new_user]').attr('value', 1); //store this so form appears right after refresh
					Drupal.behaviors.vsite_register.toggle_user_forms();
				});
			}
			
			Drupal.detachBehaviors($(this)); //we don't want to set up the hidden element on every page load

		}
  }
})(jQuery);
			