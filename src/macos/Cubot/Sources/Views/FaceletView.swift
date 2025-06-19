import Cubot
import SwiftUI

struct FaceletView: View {
    @Binding var color: cubot.Color?

    @EnvironmentObject var viewModel: CubePickerView.ViewModel
    @State private var isHovering = false

    var body: some View {
        Group {
            if let color = color {
                Rectangle()
                    .foregroundColor(color.asSwiftUIColor)
            } else {
                Rectangle()
                    .foregroundColor(.gray)
            }
        }
        .aspectRatio(1.0, contentMode: .fit)
        .border(isHovering ? .white : .clear, width: 1)
        .onTapGesture { color = viewModel.selectedColor }
        .onHover { isHovering in self.isHovering = isHovering }
    }
}
