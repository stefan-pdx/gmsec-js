FdsDemo.notifyController = SC.ObjectController.create
	notify: (header, content)  ->
		$.jGrowl content, header: header
