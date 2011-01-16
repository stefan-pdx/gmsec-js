FdsDemo.maneuverController = SC.ObjectController.create
	contentBinding: SC.Binding.single 'FdsDemo.maneuversArrayController.selection'

FdsDemo.maneuversArrayController = SC.ArrayController.create
	addManeuver: (epoch) ->
		this.content.pushObject
			epoch: epoch
			iterations: []
		this.content.length-1

	addManeuverIteration: (maneuverID, label) ->
		this.content[maneuverID].iterations.pushObject SC.Object.create
			label: label
			data: []
		this.content[maneuverID].length-1

	addDataToIteration: (maneuverID, iterationID, data) ->
		this.content[maneuverID].iterations[iterationID].data.pushObjects data
   
	content: [{
		epoch: 'Maneuver 1'
		iterations: [
			SC.Object.create
				label: 'set1'
				data:  [[1,2]
					[2,3]
					[3,4]],
			SC.Object.create
				label: 'zxcv'
				data:  [[0,0]
					[1,3]
					[2,4]]]
		},{
		epoch: 'Maneuver 2'
		iterations: [
			SC.Object.create
				label: 'set1'
				data:  [[1,4]
					[2,3]
					[3,1]],
			SC.Object.create
				label: 'zxcv'
				data:  [[0,2]
					[1,1]
					[2,6]]]}]
 
