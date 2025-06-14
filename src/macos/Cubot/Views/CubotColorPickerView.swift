import Cubot
import SwiftUI

struct CubotColorPickerView: View {
    @Binding var color: Cubot.Color?

    var body: some View {
        HStack {
            ForEach(cubotColors, id: \.self) { color in
                Circle()
                    .foregroundColor(color.asSwiftUIColor)
                    .overlay {
                        Circle()
                            .stroke(self.color == color ? .white : .clear, lineWidth: 2)
                    }
                    .onTapGesture { _ in self.color = color }
            }
        }
    }
}
