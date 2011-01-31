FdsDemo.maneuverController = SC.ObjectController.create
    contentBinding: SC.Binding.single 'FdsDemo.maneuversArrayController.selection'

FdsDemo.maneuversArrayController = SC.ArrayController.create
    maneuverCount: 0
    addManeuver: (title) ->
        maneuverCount += 1
        this.content.pushObject
            title: 'Maneuver '+FdsDemo.maneuversArrayController.maneuverCount 
            iterations: []

    addManeuverIteration: (maneuverID, label) ->
        this.content[maneuverID-1].iterations.pushObject SC.Object.create
            label: label
            data: []
        this.content[maneuverID-1].iterations.length-1

    addDataToIteration: (maneuverID, iterationID, data) ->
        this.content[maneuverID-1].iterations[iterationID-1].data.pushObjects data
   
    content: []
    ### epoch: 'Maneuver 1'
        iterations: [
            SC.Object.create
                label: 'set1'
                data:  [[1,2]
                        [2,3]
                        [3,4]],
                points: {show: true}
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
                    [2,6]]]}]###
 
