require 'jquery.jgrowl_minimized.js'

FdsDemo.notifyController = SC.ObjectController.create
	notify: (header, content)  ->
		$.jGrowl content, header: header
