import Cubot
import SwiftUI

struct FaceView: View {
    @Binding var colors: [cubot.Color?]

    @State private var popoverIndex: Int? = nil

    var body: some View {
        GeometryReader { geometry in
            let verticalSpacing = 0.05 * geometry.size.height
            let horizontalSpacing = 0.05 * geometry.size.width

            VStack(spacing: verticalSpacing) {
                ForEach(0..<3) { row in
                    HStack(spacing: horizontalSpacing) {
                        ForEach(0..<3) { column in
                            let colorIndex = row * 3 + column
                            FaceletView(color: colors[colorIndex])
                                .onTapGesture { popoverIndex = colorIndex }
                                .popover(
                                    isPresented: Binding<Bool>(
                                        get: { popoverIndex == colorIndex },
                                        set: { show in
                                            popoverIndex =
                                                show ? colorIndex : nil
                                        }
                                    )
                                ) {
                                    CubotColorPickerView(
                                        color: $colors[colorIndex]
                                    )
                                    .frame(width: 200)
                                    .padding()
                                }
                        }
                    }
                }
            }
        }

    }
}
